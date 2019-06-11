load 'data20190414'

figure(1)
plot(lambda,X_LFOS,'r-s','LineWidth',2.5,'MarkerSize',6)
hold on
plot(lambda,X_EFOS,'m-x','LineWidth',2.5,'MarkerSize',6)
plot(lambda,X_JOS1,'g-o','LineWidth',2.5,'MarkerSize',6)
plot(lambda,X_JOS5, 'b-+','LineWidth',2.5,'MarkerSize',6)
plot(lambda,X_JOS10, 'k-*','LineWidth',2.5,'MarkerSize',6)
plot(lambda,X_COS1,'g:o','LineWidth',2.5,'MarkerSize',6)
plot(lambda,X_COS5, 'b:+','LineWidth',2.5,'MarkerSize',6)
plot(lambda,X_COS10, 'k:*','LineWidth',2.5,'MarkerSize',6)
legend({'LFOS', 'EFOS','LC-JOS(M=1)', 'LC-JOS(M=5)', 'LC-JOS(M=10)','Centralized(M=1)', 'Centralized(M=5)', 'Centralized(M=10)'},'FontSize',11)
xlabel('mean arrival rate','FontSize',18)
ylabel('average virtual queue length','FontSize',18)
grid on

figure(2)
plot(lambda,(P_LFOS-P_EFOS)./P_LFOS,'m-x','LineWidth',2.5,'MarkerSize',6)
hold on
plot(lambda,(P_LFOS-P_JOS1)./P_LFOS,'g-o','LineWidth',2.5,'MarkerSize',6)
plot(lambda,(P_LFOS-P_JOS5)./P_LFOS, 'b-+','LineWidth',2.5,'MarkerSize',6)
plot(lambda,(P_LFOS-P_JOS10)./P_LFOS, 'k-*','LineWidth',2.5,'MarkerSize',6)
plot(lambda,(P_LFOS-P_COS1)./P_LFOS,'g:o','LineWidth',2.5,'MarkerSize',6)
plot(lambda,(P_LFOS-P_COS5)./P_LFOS, 'b:+','LineWidth',2.5,'MarkerSize',6)
plot(lambda,(P_LFOS-P_COS10)./P_LFOS, 'k:*','LineWidth',2.5,'MarkerSize',6)
legend({'EFOS','LC-JOS(M=1)', 'LC-JOS(M=5)', 'LC-JOS(M=10)','Centralized(M=1)', 'Centralized(M=5)', 'Centralized(M=10)'},'FontSize',11)
xlabel('Mean Arrival Rate','FontSize',18)
ylabel('Energy saving percentage(%)','FontSize',18)
grid on

figure(3)
plot(AvgX_JOS, AvgP_JOS, 'b-o','LineWidth',2.5,'MarkerSize',6)
hold on
plot(AvgX_COS, AvgP_COS, 'k:*','LineWidth',2.5,'MarkerSize',6)
plot(2.04953824,2.07273388, 'r-x', 'LineWidth',2.5,'MarkerSize',6)
plot(2.03987612,1.19058456, 'g-+', 'LineWidth',2.5,'MarkerSize',6)
legend({'LC-JOS','Centralized','LFOS','EFOS'},'FontSize',11)
xlabel('average virtual queue length','FontSize',18)
ylabel('average energy consumption','FontSize',18)
grid on
