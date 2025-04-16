
% NTC resistor Calculation

R_cold = 42317; % -5
R_hot = 2985; % 55

R_cold = 32000; % 0
R_hot = 3500; % 50


R_cold = 25395; % 5
R_hot = 4367; % 45


##
##R_cold = 32000;% 0
##R_hot = 2985;% 55



##R_cold = 32000;% 0
##R_hot = 2985;% 55


##R_hot = 3500;


##VDRV = V_in??

V_in = 5;

V_hot = V_in * 0.15;
V_cool = V_in * 0.36;
V_cold = V_in * 0.398;

R_lo = (V_in * R_cold * R_hot * (1/V_cold - 1/V_hot)) / (R_hot * ( V_in / V_hot - 1) - R_cold *  (V_in / V_cold - 1))

R_hi = ( V_in / V_hot - 1) / (1 / R_lo + 1/ R_cold)

% R1 = R_hi
% R11 = R_lo

R_cool = (R_lo * R_hi * V_cool) / (R_lo - ((R_lo * V_cool) - (R_hi - V_cool)))

V_warm = V_hot;
R_warm = (R_lo * R_hi * V_warm) / (R_lo - ((R_lo * V_warm) - (R_hi - V_warm)))
