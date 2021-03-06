%%Read data from CSV file
ImportSD = readtable('nuclear_plants_small_dataset.csv');
ImportBD = datastore('nuclear_plants_big_dataset.csv');
SD2 = removevars(ImportSD,{'Status'});

%%Splitting table into 2 tables
tNorm = ImportSD;
tAbnorm = ImportSD;
tNorm = tNorm(1:498, :);
tAbnorm = tAbnorm(499:end, :);

%%Splitting each row from 'Normal' dataset and converting it into an array
PRSN1 = tNorm.(2);
PRSN2 = tNorm.(3);
PRSN3 = tNorm.(4);
PRSN4 = tNorm.(5);
PSN1 = tNorm.(6);
PSN2 = tNorm.(7);
PSN3 = tNorm.(8);
PSN4 = tNorm.(9);
VSN1 = tNorm.(10);
VSN2 = tNorm.(11);
VSN3 = tNorm.(12);
VSN4 = tNorm.(13);

%%Splitting each row from 'Abnormal' dataset and converting it into an array
PRSAN1 = tAbnorm.(2);
PRSAN2 = tAbnorm.(3);
PRSAN3 = tAbnorm.(4);
PRSAN4 = tAbnorm.(5);
PSAN1 = tAbnorm.(6);
PSAN2 = tAbnorm.(7);
PSAN3 = tAbnorm.(8);
PSAN4 = tAbnorm.(9);
VSAN1 = tAbnorm.(10);
VSAN2 = tAbnorm.(11);
VSAN3 = tAbnorm.(12);
VSAN4 = tAbnorm.(13);

%%Calculating all summery statistics for each array of each dataset
MeanPRSN1 = mean(PRSN1);
MedianPRSN1 = median(PRSN1);
ModePRSN1 = mode(PRSN1);
VarPRSN1 = var(PRSN1);
MinPRSN1 = min(PRSN1);
MaxPRSN1 = max(PRSN1);

MeanPRSN2 = mean(PRSN2);
MedianPRSN2 = median(PRSN2);
ModePRSN2 = mode(PRSN2);
VarPRSN2 = var(PRSN2);
MinPRSN2 = min(PRSN2);
MaxPRSN2 = max(PRSN2);

MeanPRSN3 = mean(PRSN3);
MedianPRSN3 = median(PRSN3);
ModePRSN3 = mode(PRSN3);
VarPRSN3 = var(PRSN3);
MinPRSN3 = min(PRSN3);
MaxPRSN3 = max(PRSN3);

MeanPRSN4 = mean(PRSN4);
MedianPRSN4 = median(PRSN4);
ModePRSN4 = mode(PRSN4);
VarPRSN4 = var(PRSN4);
MinPRSN4 = min(PRSN4);
MaxPRSN4 = max(PRSN4);

MeanPSN1 = mean(PSN1);
MedianPSN1 = median(PSN1);
ModePSN1 = mode(PSN1);
VarPSN1 = var(PSN1);
MinPSN1 = min(PSN1);
MaxPSN1 = max(PSN1);

MeanPSN2 = mean(PSN2);
MedianPSN2 = median(PSN2);
ModePSN2 = mode(PSN2);
VarPSN2 = var(PSN2);
MinPSN2 = min(PSN2);
MaxPSN2 = max(PSN2);

MeanPSN3 = mean(PSN3);
MedianPSN3 = median(PSN3);
ModePSN3 = mode(PSN3);
VarPSN3 = var(PSN3);
MinPSN3 = min(PSN3);
MaxPSN3 = max(PSN3);

MeanPSN4 = mean(PSN4);
MedianPSN4 = median(PSN4);
ModePSN4 = mode(PSN4);
VarPSN4 = var(PSN4);
MinPSN4 = min(PSN4);
MaxPSN4 = max(PSN4);

MeanVSN1 = mean(VSN1);
MedianVSN1 = median(VSN1);
ModeVSN1 = mode(VSN1);
VarVSN1 = var(VSN1);
MinVSN1 = min(VSN1);
MaxVSN1 = max(VSN1);

MeanVSN2 = mean(VSN2);
MedianVSN2 = median(VSN2);
ModeVSN2 = mode(VSN2);
VarVSN2 = var(VSN2);
MinVSN2 = min(VSN2);
MaxVSN2 = max(VSN2);

MeanVSN3 = mean(VSN3);
MedianVSN3 = median(VSN3);
ModeVSN3 = mode(VSN3);
VarVSN3 = var(VSN3);
MinVSN3 = min(VSN3);
MaxVSN3 = max(VSN3);

MeanVSN4 = mean(VSN4);
MedianVSN4 = median(VSN4);
ModeVSN4 = mode(VSN4);
VarVSN4 = var(VSN4);
MinVSN4 = min(VSN4);
MaxVSN4 = max(VSN4);

MeanPRSAN1 = mean(PRSAN1);
MedianPRSAN1 = median(PRSAN1);
ModePRSAN1 = mode(PRSAN1);
VarPRSAN1 = var(PRSAN1);
MinPRSAN1 = min(PRSAN1);
MaxPRSAN1 = max(PRSAN1);

MeanPRSAN2 = mean(PRSAN2);
MedianPRSAN2 = median(PRSAN2);
ModePRSAN2 = mode(PRSAN2);
VarPRSAN2 = var(PRSAN2);
MinPRSAN2 = min(PRSAN2);
MaxPRSAN2 = max(PRSAN2);

MeanPRSAN3 = mean(PRSAN3);
MedianPRSAN3 = median(PRSAN3);
ModePRSAN3 = mode(PRSAN3);
VarPRSAN3 = var(PRSAN3);
MinPRSAN3 = min(PRSAN3);
MaxPRSAN3 = max(PRSAN3);

MeanPRSAN4 = mean(PRSAN4);
MedianPRSAN4 = median(PRSAN4);
ModePRSAN4 = mode(PRSAN4);
VarPRSAN4 = var(PRSAN4);
MinPRSAN4 = min(PRSAN4);
MaxPRSAN4 = max(PRSAN4);

MeanPSAN1 = mean(PSAN1);
MedianPSAN1 = median(PSAN1);
ModePSAN1 = mode(PSAN1);
VarPSAN1 = var(PSAN1);
MinPSAN1 = min(PSAN1);
MaxPSAN1 = max(PSAN1);

MeanPSAN2 = mean(PSAN2);
MedianPSAN2 = median(PSAN2);
ModePSAN2 = mode(PSAN2);
VarPSAN2 = var(PSAN2);
MinPSAN2 = min(PSAN2);
MaxPSAN2 = max(PSAN2);

MeanPSAN3 = mean(PSAN3);
MedianPSAN3 = median(PSAN3);
ModePSAN3 = mode(PSAN3);
VarPSAN3 = var(PSAN3);
MinPSAN3 = min(PSAN3);
MaxPSAN3 = max(PSAN3);

MeanPSAN4 = mean(PSAN4);
MedianPSAN4 = median(PSAN4);
ModePSAN4 = mode(PSAN4);
VarPSAN4 = var(PSAN4);
MinPSAN4 = min(PSAN4);
MaxPSAN4 = max(PSAN4);

MeanVSAN1 = mean(VSAN1);
MedianVSAN1 = median(VSAN1);
ModeVSAN1 = mode(VSAN1);
VarVSAN1 = var(VSAN1);
MinVSAN1 = min(VSAN1);
MaxVSAN1 = max(VSAN1);

MeanVSAN2 = mean(VSAN2);
MedianVSAN2 = median(VSAN2);
ModeVSAN2 = mode(VSAN2);
VarVSAN2 = var(VSAN2);
MinVSAN2 = min(VSAN2);
MaxVSAN2 = max(VSAN2);

MeanVSAN3 = mean(VSAN3);
MedianVSAN3 = median(VSAN3);
ModeVSAN3 = mode(VSAN3);
VarVSAN3 = var(VSAN3);
MinVSAN3 = min(VSAN3);
MaxVSAN3 = max(VSAN3);

MeanVSAN4 = mean(VSAN4);
MedianVSAN4 = median(VSAN4);
ModeVSAN4 = mode(VSAN4);
VarVSAN4 = var(VSAN4);
MinVSAN4 = min(VSAN4);
MaxVSAN4 = max(VSAN4);

%%Putting all the summery statistics for the normal data into a table
Names = {'PowerRangeSensor1';'PowerRangeSensor2';'PowerRangeSensor23';'PowerRangeSensor4';
    'PressureSensor1';'PressureSensor2';'PressureSensor3';'PressureSensor4';
    'VibrationSensor1';'VibrationSensor2';'VibrationSensor3';'VibrationSensor4'};
ValueMeans = [MeanPRSN1;MeanPRSN2;MeanPRSN3;MeanPRSN4;MeanPSN1;MeanPSN2;MeanPSN3;MeanPSN4;MeanVSN1;MeanVSN2;MeanVSN3;MeanVSN4];
ValueMedians = [MedianPRSN1;MedianPRSN2;MedianPRSN3;MedianPRSN4;MedianPSN1;MedianPSN2;MedianPSN3;MedianPSN4;MedianVSN1;MedianVSN2;MedianVSN3;MedianVSN4];
ValueModes = [ModePRSN1;ModePRSN2;ModePRSN3;ModePRSN4;ModePSN1;ModePSN2;ModePSN3;ModePSN4;ModeVSN1;ModeVSN2;ModeVSN3;ModeVSN4];
ValueMin = [MinPRSN1;MinPRSN2;MinPRSN3;MinPRSN4;MinPSN1;MinPSN2;MinPSN3;MinPSN4;MinVSN1;MinVSN2;MinVSN3;MinVSN4];
ValueMax = [MaxPRSN1;MaxPRSN2;MaxPRSN3;MaxPRSN4;MaxPSN1;MaxPSN2;MaxPSN3;MaxPSN4;MaxVSN1;MaxVSN2;MaxVSN3;MaxVSN4];
ValueVariance = [VarPRSN1;VarPRSN2;VarPRSN3;VarPRSN4;VarPSN1;VarPSN2;VarPSN3;VarPSN4;VarVSN1;VarVSN2;VarVSN3;VarVSN4];

tNormSum = table(Names,ValueMin,ValueMax,ValueMeans,ValueMedians,ValueModes,ValueVariance);

NamesAb = {'PowerRangeSensor1';'PowerRangeSensor2';'PowerRangeSensor23';'PowerRangeSensor4';
    'PressureSensor1';'PressureSensor2';'PressureSensor3';'PressureSensor4';
    'VibrationSensor1';'VibrationSensor2';'VibrationSensor3';'VibrationSensor4'};
ValueMeansAb = [MeanPRSAN1;MeanPRSAN2;MeanPRSAN3;MeanPRSAN4;MeanPSAN1;MeanPSAN2;MeanPSAN3;MeanPSAN4;MeanVSAN1;MeanVSAN2;MeanVSAN3;MeanVSAN4];
ValueMediansAb = [MedianPRSAN1;MedianPRSAN2;MedianPRSAN3;MedianPRSAN4;MedianPSAN1;MedianPSAN2;MedianPSAN3;MedianPSAN4;MedianVSAN1;MedianVSAN2;MedianVSAN3;MedianVSAN4];
ValueModesAb = [ModePRSAN1;ModePRSAN2;ModePRSAN3;ModePRSAN4;ModePSAN1;ModePSAN2;ModePSAN3;ModePSAN4;ModeVSAN1;ModeVSAN2;ModeVSAN3;ModeVSAN4];
ValueMinAb = [MinPRSAN1;MinPRSAN2;MinPRSAN3;MinPRSAN4;MinPSAN1;MinPSAN2;MinPSAN3;MinPSAN4;MinVSAN1;MinVSAN2;MinVSAN3;MinVSAN4];
ValueMaxAb = [MaxPRSAN1;MaxPRSAN2;MaxPRSAN3;MaxPRSAN4;MaxPSAN1;MaxPSAN2;MaxPSAN3;MaxPSAN4;MaxVSAN1;MaxVSAN2;MaxVSAN3;MaxVSAN4];
ValueVarianceAb = [VarPRSAN1;VarPRSAN2;VarPRSAN3;VarPRSAN4;VarPSAN1;VarPSAN2;VarPSAN3;VarPSAN4;VarVSAN1;VarVSAN2;VarVSAN3;VarVSAN4];

tAbnormSum = table(NamesAb,ValueMinAb,ValueMaxAb,ValueMeansAb,ValueMediansAb,ValueModesAb,ValueVarianceAb);

%%creating a corrolation matrix for the initial dataset
corrMatrix = corr(SD2.Variables);

%%Randomizing initial data file and splitting it into test and training data
ranrows = randperm(height(ImportSD));
TrainingD = ImportSD(ranrows(1:0.7*height(ImportSD)), :);
TestD = ImportSD(ranrows(0.7*height(ImportSD)+1:end), :);

%%Creating classification tree
dTree = fitctree(TrainingD(:,2:end),TrainingD(:,1));        
predictLabels = predict(dTree,TestD(:,2:end));    
trueLabels = TestD(:,1);
%%Con = confusionchart(trueLabels,predictLabels)
%%Con = plotconfusion(trueLabels,predictLabels);
%%Loss = resubLoss(dTree);

svmC = fitcsvm(TrainingD(:,2:end),TrainingD(:,1));
predictsvmC = predict(svmC,TestD(:,2:end));
truesvmC = TestD(:,1);

%%ANN = train(TrainingD,X,T,Xi,Ai,EW);

clearvars -except ImportSD tNorm tAbnorm tNormSum tAbnormSum PRSN1 PRSN2 PRSN3 PRSN4 PSN1 PSN2 PSN3 PSN4 VSN1 VSN2 VSN3 VSN4 PRSAN1 PRSAN2 PRSAN3 PRSAN4 PSAN1 PSAN2 PSAN3 PSAN4 VSAN1 VSAN2 VSAN3 VSAN4 corrMatrix SD2 TestD TrainingD dTree predictLabels trueLabels dTCM svmC predictsvmC truesvmC ImportBD