%% RAW DATA: left/right motor's RPM for PWM = 85;
clear
data = readtable("PWM_85.csv");

speedLeft85PWM = data.("Var1");
speedRight85PWM = data.("Var2");

%%
clear
data = readtable("PWM_255.csv");

speedLeft255PWM = data.("Var1");
speedRight255PWM = data.("Var2");

plot(speedLeft255PWM), xlabel("time"),ylabel("RPM");
title("RPM for left motor for 255 PWM value");

figure,plot(speedRight255PWM), xlabel("time"),ylabel("RPM");
title("RPM for right motor for 255 PWM value");
%% filtered signals for PWM = 85
load("filtered_signal_left85.mat") % imported from PhyCharm
filtered_signal_left = signal;
load("filtered_signal_right85.mat") % imported from PhyCharm
filtered_signal_right = signal;
%%
close all
plot(time,speedLeft85PWM)
hold on
plot(time,filtered_signal_left)

figure
plot(time,speedRight85PWM)
hold on
plot(time,filtered_signal_right)
%% left motor model - identificare on PWM 85 (median filtered)
u = zeros(1,196);
u(20:180) = 2.47;

u_id = u;
y_id = filtered_signal_left;

yss = 60;
uss = 2.47;

K = yss / uss;
t0 = 20;
t1 = 21;
T = (t1 - t0) / 100;

H = tf(K,[T,1]);

y_pred = lsim(H,u_id,time);

data_id = iddata(filtered_signal_left(:),u_id(:),0.01);
model = iddata(y_pred(:),u_id(:),0.01);
figure, compare(data_id,model), title("Training: Filtered identification data vs model for left motor at PWM = 85")
% K = 24.2915, T = 0.01 
%%
clear
data = readtable("PWM_255.csv");

speedRight255PWM = data.("Var2");
%% validation on PWM 255 working: H = K / (T * s + 1), K = 24.2915, T = 0.01
load("filtered_signal_left255.mat")
signal_val = signal;
u_val = zeros(1,153);
u_val(9:145) = 7.4; 

y_val = lsim(H,u_val,time);
plot(signal_val), hold on, plot(y_val)
title("Validation: Filtered validation data vs model for left motor at PWM = 255")
%% Tuning the PID controller
clear, close all
K = 24.2915;
T = 0.01;
H = tf(K,[T,1]);
% bode(H)
H0 = feedback(H,1);
% step(H0) % Estp = 0.04;


C = pidtune(H, 'PID');

Hd = series(C,H);
% bode(Hd)
H01 = feedback(Hd,1);
figure,step(H01); title("Simulare model motor stang cu regulatorulul PI")

t = 0 : 0.1 : 50;
% figure
% lsim(H01,t.^2,t)