clear all; clc; close all;

% Simulation parameters
Fs = 48000; 
Ts = 1/Fs;
cutoff = 500; % Hz
normCutoff = cutoff / Fs;

% variables

theta = pi * normCutoff;
omega = pi * cutoff;
omegaSquared = omega^2;
kappa = omega / tan(theta);
kappaSquared = kappa^2;
delta = kappaSquared + omegaSquared + (2 * kappa * omega);

%%===HPF==================
% coefficients

B0 = kappaSquared / delta;
B1 = (-2.0 * kappaSquared) / delta;
B2 = B0;
A1 = (-2.0 * kappaSquared + 2.0 * omegaSquared) / delta;
A2 = (-2.0 * kappa * omega + kappaSquared + omegaSquared) / delta;

% transfer function
num = [B0 B1 B2];
den = [1 A1 A2];

% plot
[H_hp, F] = freqz(num, den, 2048, Fs);
semilogx(F, 20*log10(abs(H_hp)));
hold on

%%===LPF==================
% coefficients

B0 = omegaSquared / delta;
B1 = 2.0 * (omegaSquared / delta);
B2 = B0;
A1 = (-2.0 * kappaSquared + 2.0 * omegaSquared) / delta;
A2 = (-2.0 * kappa * omega + kappaSquared + omegaSquared) / delta;

% transfer function
num = [B0 B1 B2];
den = [1 A1 A2];

% plot
[H_lp, F] = freqz(num, den, 2048, Fs);
semilogx(F, 20*log10(abs(H_lp)));

%%===SUMMING==================
% Invert LPF phase and sum
H_sum = H_hp - H_lp;

% Plot
semilogx(F, 20*log10(abs(H_sum)));

% Set axis limits and labels
axis([0 20000 -30 2]);
title('Linkwitz-Riley Crossover Pair','interpreter','latex','FontSize', 30); 
xlabel('Frequency (Hz)', 'interpreter', 'latex', 'FontSize', 30);
ylabel('Magnitude (dB)','interpreter', 'latex', 'FontSize', 30);
legend('HPF', 'LPF', 'Sum');

% plot phase response
figure;
phase = unwrap(angle(H_sum));
semilogx(F, phase);
axis([0 20000 -pi pi]);
xlabel('Frequency (Hz)','interpreter', 'latex', 'FontSize', 30);
ylabel('Phase (rad)','interpreter', 'latex', 'FontSize', 30);
legend('APF');
title('Linkwitz-Riley Phase Response','interpreter', 'latex', 'FontSize', 30);

hold off
