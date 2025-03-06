clear
data = readtable("PWM_85.csv");

speedRight85PWM = data.("Var2");

load("filtered_signal_right85.mat") % imported from PhyCharm: identification signal
filtered_signal_right = signal;

plot(speedRight85PWM), hold on, plot(filtered_signal_right)
%% identificare right motor PWM = 85
close all
u = zeros(1,196);
u(16:177) = 2.47;

u_id = u;
y_id = filtered_signal_right;
data_id = iddata(y_id(:),u_id(:),0.01);
model_arx = arx(data_id,[1 1 0]);

yss = mean(y_id(30:173));
uss = 2.47;

K = yss / uss;
t0 = 16;
t1 = 22;
T = (t1 - t0) / 100;

H = tf(K,[T,1]); % K = 30.1339 & T = 0.06

y_pred = lsim(H,u_id,time);

data_id = iddata(filtered_signal_right(:),u_id(:),0.01);
model = iddata(y_pred(:),u_id(:),0.01);
figure, compare(data_id,model), title("Training: Filtered identification data vs model for right motor at PWM = 85")
%% validation right motor PWM = 128
load("filtered_signal_right128.mat")
signal_val = signal;
u_val = zeros(1,136);
u_val(1:136) = 3.71;

N = length(signal_val);
Ts = mean(diff(time));
time_corrected = (0:N-1) * Ts;

y_val = lsim(H,u_val,time_corrected);

plot(signal_val),hold on, plot(y_val)
title("Validation: Filtered validation data vs model for right motor at PWM = 128")
legend("real","prediction")
%% tuning the PID controller
clear
K = 22.1452;
T = 0.04;
H = tf(K,[T,1]);
Hc = pidtune(H,'PID');
Hd = series(Hc,H);
H0 = feedback(Hd,1);
figure,step(H0); title("Simulare model motor drept cu regulatorulul PI")

