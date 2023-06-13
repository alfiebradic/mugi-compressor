clear all; clc; close all;

% fc1    fc2
%
% LP1 -> AP2
%
% HP1 -> HP2
%     -> LP2
%

% Simulation parameters
Fs = 48000; 
Ts = 1/Fs;
cutoff = 500; % Hz
normCutoff = cutoff / Fs;

% variables
alpha = (tan(pi * normCutoff) - 1) / (tan(pi * normCutoff) + 1);

%%===HPF==================
% coefficients
B0 = alpha;
B1 = 1.0;
B2 = 0.0;
A1 = alpha;
A2 = 0.0;

% transfer function
num = [B0 B1 B2];
den = [1 A1 A2];

% plot magnitude response
figure;
[H_hp, F] = freqz(num, den, 2048, Fs);
semilogx(F, 20*log10(abs(H_hp)));
axis([0 20000 -30 2]);
xlabel('Frequency (Hz)','interpreter','latex','FontSize', 30);
ylabel('Magnitude (dB)','interpreter','latex','FontSize', 30);
legend('APF');
title('APF Magnitude Response','interpreter','latex','FontSize', 30); 

% plot phase response
figure;
phase = unwrap(angle(H_hp));
semilogx(F, phase);
axis([0 20000 -pi pi]);
xlabel('Frequency (Hz)','interpreter','latex','FontSize', 30);
ylabel('Phase (rad)','interpreter','latex','FontSize', 30);
legend('APF');
title('APF Phase Response','interpreter','latex','FontSize', 30); 
