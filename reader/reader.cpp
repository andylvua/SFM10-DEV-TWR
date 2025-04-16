#include <QApplication>
#include <QtSerialPort/QSerialPort>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QVBoxLayout>
#include <QScatterSeries>
#include <QDateTime>
#include <QElapsedTimer>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

#include <iostream>
#include <fstream>
#include <iomanip>

#define DEBUG 0

typedef enum cir_type {
    COMPLEX, MAGNITUDE, COMPRESSED
} cir_type_t;

template<cir_type_t>
struct cir_type_trait;

template<>
struct cir_type_trait<COMPLEX> {
    using receive_type = int16_t;
    using log_type = int16_t;
};

template<>
struct cir_type_trait<MAGNITUDE> {
    using receive_type = float;
    using log_type = float;
};

template<>
struct cir_type_trait<COMPRESSED> {
    using receive_type = uint8_t;
    using log_type = int16_t;
};

struct Config {
    static constexpr int BAUD_RATE = 2400000;

    static char PORT_NAME[32];
    static constexpr char OUTPUT_FILE[] = "../receivedData.txt";

    static constexpr cir_type_t CIR_TYPE = COMPRESSED;
    static constexpr auto ACC_SAMPLES = 136;

    static constexpr char PREFIX[] = "UWB";
    static constexpr char SUFFIX[] = "END";
    static constexpr int TIMEOUT = 500000000;

    static constexpr bool READ_CIR = true;

    static constexpr auto CIR_LENGTH = CIR_TYPE == MAGNITUDE ? ACC_SAMPLES : 2 * ACC_SAMPLES;
};

#define VIEWER_ENABLED true
#define VISUALIZE_CIR false
#define VISUALIZE_DISTANCE false


#include "data_types.h"

using dataStruct_t = uwbLogMessageData_t;

struct Stats {
    std::map<uint16_t, uint8_t> lastRangeNumbers;
    std::map<uint16_t, int> receiveCounts;

    int correctReceives;
    int incorrectReceives;
    int negativeDistances;

    int losReceives;

    [[nodiscard]] float getErrorRate() const {
        return static_cast<float>(incorrectReceives * 100) / static_cast<float>(correctReceives + incorrectReceives);
    }
};

class CIRViewer : public QWidget {
public:
    explicit CIRViewer(QWidget *parent = nullptr)
        : QWidget(parent), isFrozen(false) {
        resize(1600, 1200);
        auto *layout = new QVBoxLayout(this);
        freezeButton = new QPushButton("Freeze", this);
        layout->addWidget(freezeButton);
        connect(freezeButton, &QPushButton::clicked, this, &CIRViewer::toggleFreeze);

        losRatioLabel = new QLabel("NLoS Ratio: 0%", this);
        QFont labelFont = losRatioLabel->font();
        labelFont.setBold(true);
        labelFont.setPointSize(24);
        losRatioLabel->setFont(labelFont);
        layout->addWidget(losRatioLabel);

        transactionsCountLabel = new QLabel("Transactions: 0", this);
        QFont labelFont = transactionsCountLabel->font();
        labelFont.setBold(true);
        labelFont.setPointSize(24);
        losRatioLabel->setFont(labelFont);
        layout->addWidget(losRatioLabel);

        setLayout(layout);

        distanceTimer = new QTimer(this);
        connect(distanceTimer, &QTimer::timeout, this, &CIRViewer::updateDistanceGraphs);
        distanceTimer->start(20);

        offlineCheckTimer = new QTimer(this);
        connect(offlineCheckTimer, &QTimer::timeout, this, &CIRViewer::checkOfflineAnchors);
        offlineCheckTimer->start(500);
    }

    void updateChart(const uint16_t &anchorAddress,
                     const cir_type_trait<Config::CIR_TYPE>::log_type *data,
                     const int distance = 0,
                     const bool los = false,
                     const Stats &stats = {}) {
        if (isFrozen) {
            return;
        }

        if (!anchors.contains(anchorAddress)) {
            addAnchor(anchorAddress);
        }

        auto &anchorData = anchors[anchorAddress];

    #if VISUALIZE_CIR
        updateCIRPlot(anchorData, data);
    #endif

        updateDistancePlot(anchorData, distance);
        updateLOSIndicator(anchorData, los);

        // Update LoS and NLoS ratio (take info from stats)
        const int losCount = stats.losReceives;
        const int nlosCount = stats.correctReceives - losCount;

        const auto nlosRatio = QString("NLoS Ratio: %1%")
        .arg(nlosCount * 100 / (losCount + nlosCount));
        losRatioLabel->setText(nlosRatio);

        anchorData.samples++;
        anchorData.lastUpdate = QDateTime::currentMSecsSinceEpoch();

        if (stats.correctReceives % 500 == 0) {
            transactionsCountLabel->setText(QString("Transactions: %1").arg(stats.correctReceives));
        }
    }

private:
    struct AnchorData {
    #if VISUALIZE_CIR
        QVector<double> cirData;
        QChart *cirChart{};
        QChartView *cirChartView{};
        QLineSeries *cirSeries{};
    #endif

    #if VISUALIZE_DISTANCE
        QChart *distanceChart{};
        QChartView *distanceChartView{};
        QLineSeries *distanceSeries{};
    #endif

        QFrame *losIndicator{};
        QLabel *anchorLabel{};
        QLabel *distanceLabel{};
        QVector<int> distancePoints;

        int samples = 0;
        qint64 lastUpdate = QDateTime::currentMSecsSinceEpoch();
        bool isOffline = true;
        bool flash = false;
    };

    QMap<uint16_t, AnchorData> anchors;
    QLabel *losRatioLabel;
    QPushButton *freezeButton;
    QTimer *distanceTimer;
    QTimer *offlineCheckTimer;
    bool isFrozen;

    void addAnchor(const uint16_t &anchorAddress) {
        auto *mainLayout = qobject_cast<QVBoxLayout *>(this->layout());
        AnchorData anchorData;

        QWidget *anchorWidget = new QWidget(this);
        QVBoxLayout *anchorLayout = new QVBoxLayout(anchorWidget);
        anchorWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Allow it to expand

        mainLayout->addWidget(anchorWidget);

        QHBoxLayout *headerLayout = new QHBoxLayout(anchorWidget);
        // make all elements in the headerLayout to be aligned to left
        anchorLayout->addLayout(headerLayout);

        anchorData.anchorLabel = new QLabel("Anchor: " + QString::number(anchorAddress, 16), this);
        QFont anchorFont = anchorData.anchorLabel->font();
        anchorFont.setBold(true);
        anchorFont.setPointSize(24);
        anchorData.anchorLabel->setFont(anchorFont);
        headerLayout->addWidget(anchorData.anchorLabel);

        anchorData.losIndicator = new QFrame(this);
        anchorData.losIndicator->setFixedSize(100, 50);
        anchorData.losIndicator->setStyleSheet("background-color: red; border: 2px solid black;");
        headerLayout->addWidget(anchorData.losIndicator, 0, Qt::AlignCenter);

        anchorData.distanceLabel = new QLabel("Distance: 0", this);
        QFont labelFont = anchorData.distanceLabel->font();
        labelFont.setBold(true);
        labelFont.setPointSize(24);
        anchorData.distanceLabel->setFont(labelFont);

        headerLayout->addWidget(anchorData.distanceLabel, 0, Qt::AlignRight);

        QHBoxLayout *chartLayout = new QHBoxLayout(anchorWidget);
        anchorLayout->addLayout(chartLayout);

    #if VISUALIZE_CIR
        anchorData.cirChart = new QChart();
        anchorData.cirChartView = new QChartView(anchorData.cirChart, this);
        anchorData.cirChartView->setRenderHint(QPainter::Antialiasing);
        chartLayout->addWidget(anchorData.cirChartView);


        anchorData.cirSeries = new QLineSeries();
        anchorData.cirSeries->setName("CIR " + QString::number(anchorAddress, 16));

        QPen cirPen(Qt::red);
        cirPen.setWidth(3);
        anchorData.cirSeries->setPen(cirPen);

        anchorData.cirChart->addSeries(anchorData.cirSeries);
        anchorData.cirChart->createDefaultAxes();

        anchorData.cirChart->setAnimationOptions(QChart::SeriesAnimations);
        anchorData.cirChart->setAnimationDuration(300);
        anchorData.cirChart->axes(Qt::Horizontal).first()->setRange(0, Config::ACC_SAMPLES);
        anchorData.cirChart->axes(Qt::Vertical).first()->setRange(0, 1);
        anchorData.cirChart->axes(Qt::Horizontal).first()->setTitleText("Sample Index");
        anchorData.cirChart->axes(Qt::Vertical).first()->setTitleText("Amplitude");

        anchorData.cirData.resize(Config::ACC_SAMPLES);
    #endif

    #if VISUALIZE_DISTANCE
        anchorData.distanceChart = new QChart();
        anchorData.distanceChartView = new QChartView(anchorData.distanceChart, this);
        anchorData.distanceChartView->setRenderHint(QPainter::Antialiasing);
        chartLayout->addWidget(anchorData.distanceChartView);

        anchorData.distanceSeries = new QLineSeries();
        anchorData.distanceSeries->setName("Distance " + QString::number(anchorAddress, 16));

        QPen distancePen(Qt::blue);
        distancePen.setWidth(3);
        anchorData.distanceSeries->setPen(distancePen);

        anchorData.distanceChart->addSeries(anchorData.distanceSeries);
        anchorData.distanceChart->createDefaultAxes();
        anchorData.distanceChart->axes(Qt::Horizontal).first()->setTitleText("Sample");
        anchorData.distanceChart->axes(Qt::Vertical).first()->setTitleText("Distance");
        anchorData.distanceChart->axes(Qt::Vertical).first()->setRange(0, 200);

    #endif

        anchors.insert(anchorAddress, anchorData);

        // Ensure all anchors split available height equally
        for (int i = 0; i < mainLayout->count(); ++i) {
            mainLayout->setStretch(i, 1);  // Give equal stretch to each anchor widget
        }
    }

#if VISUALIZE_CIR
    static void updateCIRPlot(AnchorData &anchorData, const cir_type_trait<Config::CIR_TYPE>::log_type *data) {
        QVector<double> cirBuffer;
        cirBuffer.reserve(Config::ACC_SAMPLES);

        if constexpr (Config::CIR_TYPE == COMPLEX || Config::CIR_TYPE == COMPRESSED) {
            for (int i = 0; i < Config::ACC_SAMPLES * 2; i += 2) {
                cirBuffer.append(
                    std::sqrt(data[i] * data[i] + data[i + 1] * data[i + 1])
                );
            }
        } else {
            for (int i = 0; i < Config::ACC_SAMPLES; ++i) {
                cirBuffer.append(data[i]);
            }
        }

        auto max = *std::max_element(cirBuffer.begin(), cirBuffer.end());
        for (double & i : cirBuffer) {
            i /= max;
        }

        for (int i = 0; i < cirBuffer.size(); ++i) {
            anchorData.cirData[i] += cirBuffer[i];
        }

        if (anchorData.samples % 10 == 0) {
            anchorData.cirSeries->clear();

            for (int i = 0; i < Config::ACC_SAMPLES; ++i) {
                anchorData.cirSeries->append(i, anchorData.cirData[i] / 10);
            }

            anchorData.cirData.fill(0);
        }
    }
#endif

    static void updateDistancePlot(AnchorData &anchorData, int distance) {
        if (anchorData.distancePoints.size() >= 500) {
            anchorData.distancePoints.pop_front();
#if VISUALIZE_DISTANCE
            anchorData.distanceSeries->remove(0);
#endif
        }

        anchorData.distancePoints.append(distance);
        double averagedDistance = std::accumulate(anchorData.distancePoints.end() - std::min(50, (int)anchorData.distancePoints.size()), anchorData.distancePoints.end(), 0.0) / std::min(50, (int)anchorData.distancePoints.size());
        anchorData.distanceLabel->setText(QString("Distance: %1").arg(static_cast<int>(averagedDistance)));
#if VISUALIZE_DISTANCE
        anchorData.distanceSeries->append(anchorData.samples, averagedDistance);
#endif
    }

    static void updateLOSIndicator(const AnchorData &anchorData, const bool los) {
        if (los) {
            anchorData.losIndicator->setStyleSheet("background-color: green; border: 2px solid black;");
        } else {
            anchorData.losIndicator->setStyleSheet("background-color: red; border: 2px solid black;");
        }
    }

    void toggleFreeze() {
        isFrozen = !isFrozen;
        freezeButton->setText(isFrozen ? "Unfreeze" : "Freeze");
    }

    void updateDistanceGraphs() const {
        if (isFrozen) return;
#if VISUALIZE_DISTANCE
        for (auto &anchorData : anchors) {
            auto &distanceChart = anchorData.distanceChart;
            distanceChart->axes(Qt::Horizontal).first()->setRange(anchorData.samples - 500, anchorData.samples);
        }
#endif
    }

    void checkOfflineAnchors() {
        const qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        for (auto &anchor : anchors) {
            if (currentTime - anchor.lastUpdate > 1000) {
                if (!anchor.isOffline) {
                    anchor.isOffline = true;
                    updateAnchorLabelColor(anchor);
                }
            } else {
                if (anchor.isOffline) {
                    anchor.isOffline = false;
                    updateAnchorLabelColor(anchor);
                }
            }
        }
    }

    void updateAnchorLabelColor(AnchorData &anchor) {
        if (!anchor.isOffline) {
            anchor.anchorLabel->setStyleSheet("color: #1fd655; font-weight: bold;");
        } else {
            anchor.flash = !anchor.flash;
            const QString color = anchor.flash ? "red" : "white";
            anchor.anchorLabel->setStyleSheet(QString("color: %1; font-weight: bold;").arg(color));
            QTimer::singleShot(500, this, [this, &anchor]() { updateAnchorLabelColor(anchor); });
        }
    }
};



class SerialReceiver : public QObject {
public:
    explicit SerialReceiver(CIRViewer *viewer = nullptr)
        : dataViewer(viewer), state(State::WAITING_PREFIX) {

        serial.setPortName(Config::PORT_NAME);
        serial.setBaudRate(Config::BAUD_RATE);

        if (!serial.open(QIODevice::ReadWrite)) {
            qDebug() << "Error: " << serial.errorString();
            exit(1);
        }

        outFile.open(Config::OUTPUT_FILE);
        process();
    }

private:
    static void deltaDecode(const uint8_t* receivedData, int16_t* decodedData) {
        decodedData[0] = receivedData[0] << 8 | receivedData[1] & 0xFF;
        decodedData[1] = receivedData[2] << 8 | receivedData[3] & 0xFF;

        size_t decodeIndex = 4;

        for (size_t i = 2; i < Config::CIR_LENGTH; i += 1) {
            if (receivedData[decodeIndex] & 0x80) {
                decodedData[i] = (receivedData[decodeIndex] & 0x7F) << 8 | receivedData[decodeIndex + 1];
                if (decodedData[i] & 0x4000) {
                    decodedData[i] |= 0xFFFF8000;
                }
                decodeIndex += 2;
            } else {
                decodedData[i] = receivedData[decodeIndex++];
                if (decodedData[i] & 0x40) {
                    decodedData[i] |= 0xFF80;
                }
            }
            decodedData[i] += decodedData[i - 2];
        }
    }

    bool waitReadyRead(int bytes) {
        timer.start();

        while (serial.bytesAvailable() < bytes) {
            QCoreApplication::processEvents();
            if (timer.elapsed() > Config::TIMEOUT) {
                state = State::TERMINATED;
                return false;
            }
        }

        timer.invalidate();
        return true;
    }

    void waitPrefix() {
        QElapsedTimer timer;
        timer.start();

        QString buffer;

        for (int i = 0; i < sizeof(Config::PREFIX) - 1; ++i) {
            waitLoop:
            if (timer.elapsed() > Config::TIMEOUT) {
                state = State::TERMINATED;
                return;
            }

            if (!waitReadyRead(sizeof(char))) {
                return;
            }

            char c = serial.read(sizeof(char)).at(0);
            buffer.append(c);

            if (c == '\n') {
                if (!buffer.startsWith("UWB")) {
                    qDebug() << "\nReceived: " << buffer;
                }
                buffer.clear();
                continue;
            }

            if (c == Config::PREFIX[i]) {
                continue;
            }
            i = 0;
            goto waitLoop;
        }

        buffer.clear();
        state = State::RECEIVING_DATA;
        timer.invalidate();
    }

    void receiveData() {
        if (!waitReadyRead(sizeof(dataStruct_t) + 1)) { // +1 for XOR byte
            return;
        }

        dataStruct_t rx_data;
        QByteArray dataBytes = serial.read(sizeof(dataStruct_t));
        memcpy(&rx_data, dataBytes.data(), sizeof(dataStruct_t));

        uint8_t dataXor = 0x55;
        for (int i = 0; i < sizeof(dataStruct_t); ++i) {
            dataXor ^= dataBytes.at(i);
        }

        uint8_t receivedXor = serial.read(sizeof(uint8_t)).at(0);
        if (dataXor != receivedXor) {
            qDebug() << "Invalid XOR byte";
            stats.incorrectReceives++;
            state = State::WAITING_PREFIX;
            return;
        }

        if (stats.receiveCounts[rx_data.anchor] > 0) {
            uint8_t next = stats.lastRangeNumbers[rx_data.anchor] + 1;
            if (next != rx_data.rangeNumber) {
                qDebug() << "\nSkipped ranging exchange";
                qDebug() << "Expected:" << next << "received: " << rx_data.rangeNumber
                << " from anchor: " << Qt::hex << rx_data.anchor << Qt::dec;
                stats.incorrectReceives++;
            }
        }

        if (rx_data.distance < 0) {
            stats.negativeDistances++;
        }

        this->data = rx_data;
        stats.lastRangeNumbers[this->data.anchor] = this->data.rangeNumber;

        if constexpr (Config::READ_CIR) {
            state = State::RECEIVING_CIR;
        } else {
            state = State::RECEIVING_SUFFIX;
        }
    }

    void receiveCIR() {
        using cir_type = cir_type_trait<Config::CIR_TYPE>::receive_type;

        uint16_t cir_bytes;

        if (!waitReadyRead(sizeof(uint16_t))) {
            return;
        }

        serial.read(reinterpret_cast<char *>(&cir_bytes), sizeof(uint16_t));

        cir_type receivedArray[cir_bytes / sizeof(cir_type)];

        size_t bytesReceived = 0;
        while (bytesReceived < cir_bytes) {
            if (!waitReadyRead(sizeof(cir_type))) {
                return;
            }

            cir_type value;
            serial.read(reinterpret_cast<char *>(&value), sizeof(cir_type));

            receivedArray[bytesReceived / sizeof(cir_type)] = value;
            bytesReceived += sizeof(cir_type);
        }

        if constexpr (Config::CIR_TYPE == COMPRESSED) {
            int16_t decodedData[Config::CIR_LENGTH];
            deltaDecode(reinterpret_cast<uint8_t *>(receivedArray), decodedData);

            for (int i = 0; i < Config::CIR_LENGTH; ++i) {
                cirData[i] = decodedData[i];
            }
        } else {
            for (int i = 0; i < Config::CIR_LENGTH; ++i) {
                cirData[i] = receivedArray[i];
            }
        }

        state = State::RECEIVING_SUFFIX;
    }

    void receiveSuffix() {
        if (!waitReadyRead(sizeof(Config::SUFFIX) - 1)) {
            return;
        }

        QByteArray suffix = serial.read(sizeof(Config::SUFFIX) - 1);
        if (suffix != Config::SUFFIX) {
            qDebug() << "\nInvalid suffix: " << suffix;
            stats.incorrectReceives++;
            state = State::WAITING_PREFIX;
            this->data = {};
            return;
        }

        stats.correctReceives++;
        stats.losReceives += data.losStatus == 1;
        stats.receiveCounts[this->data.anchor]++;

        if (firstReceive) {
            runningTimer.start();
            firstReceive = false;
        }

        if (dataViewer && data.status == 0) {
            dataViewer->updateChart(data.anchor, cirData, data.distance, data.losStatus == 1, stats);
        }

        writeLog();

        if (stats.correctReceives % 10 == 0) {
            std::cout << "\rRunning: Transactions count: " << stats.correctReceives
            << " | Time elapsed: " << runningTimer.elapsed() / 1000 << "s"
            << " | Negative distances: " << stats.negativeDistances
            << " | Frequency: " << std::fixed << std::setprecision(3) <<
            static_cast<float>(stats.correctReceives) / (runningTimer.elapsed() / 1000) << "Hz"
            << " | Error rate: " << std::fixed << std::setprecision(3) <<
                stats.getErrorRate() << "%";
            for (auto const& [anchor, count] : stats.receiveCounts) {
                std::cout << std::hex << " | Anchor " << anchor << std::dec << ": " << count;
            }
            std::cout << std::flush;
        }

        state = State::WAITING_PREFIX;
    }

    void writeLog() {
        outFile
        << "{'a':" << "'" << std::hex << data.anchor << std::dec << "'"
        << ",'t':" << data.timestamp
        << ",'D':" << data.distance
        << ",'n':" << std::to_string(data.rangeNumber)
        << ",'s':" << std::to_string(data.status)
        // << ",'l':" << std::to_string(data.losStatus)
        // << ",'tt':" << data.transactionTime
        // << ",'tt':" << data.transactionTime
        // << ",'sc':" << data.lastSlotCorrection
        // << ",'gt':" << data.guardTime
        ;

        if constexpr (Config::READ_CIR) {
            outFile << ",'CIR':[";
            for (int i = 0; i < Config::CIR_LENGTH; ++i) {
                outFile << cirData[i];
                if (i != Config::CIR_LENGTH - 1) {
                    outFile << ",";
                }
            }
            outFile << "]}" << std::endl;
        } else {
            outFile << "}" << std::endl;
        }
    }

    void process() {
        while (true) {
            switch (state) {
                case State::WAITING_PREFIX:
                    waitPrefix();
                    break;
                case State::RECEIVING_DATA:
                    receiveData();
                    break;
                case State::RECEIVING_CIR:
                    receiveCIR();
                    break;
                case State::RECEIVING_SUFFIX:
                    receiveSuffix();
                    break;
                case State::TERMINATED:
                    runningTimer.invalidate();
                    qDebug() << "Timeout occurred. Wait or exit? ([w]/e)";
                    char c;
                    std::cin >> c;
                    if (c == 'e') {
                        outFile.close();
                        exit(0);
                    }

                    state = State::WAITING_PREFIX;
                    break;
            }
        }
    }

    enum class State {
        WAITING_PREFIX, RECEIVING_DATA, RECEIVING_CIR, RECEIVING_SUFFIX, TERMINATED
    };

    QSerialPort serial;
    QElapsedTimer timer;
    QElapsedTimer runningTimer;
    CIRViewer *dataViewer;

    std::ofstream outFile;

    State state;
    Stats stats{};

    cir_type_trait<Config::CIR_TYPE>::log_type cirData[Config::CIR_LENGTH]{};

    dataStruct_t data{};
    bool firstReceive = true;
};

// Initialize static members
char Config::PORT_NAME[32];

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    if (argc > 1) {
        strcpy(Config::PORT_NAME, argv[1]);
    } else {
        strcpy(Config::PORT_NAME, "/dev/ttyUSB0");
    }

    CIRViewer *viewer = nullptr;

    if (VIEWER_ENABLED) {
        viewer = new CIRViewer();
        viewer->resize(800, 600);
        viewer->show();
    }

    SerialReceiver receiver(viewer);

    return QApplication::exec();
}
