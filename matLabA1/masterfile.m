%%Read data from CSV file
ImportT = readtable('car_data.csv');

%%Putting table data into arrays
MPG = ImportT.(1);
Cyl = ImportT.(2);
Disp = ImportT.(3);
HP = ImportT.(4);
Weight = ImportT.(5);
Acceleration = ImportT.(6);
%%Finding NaN values in one of two columns with missing values, and
%%replacing them with that columns mean value excluding NaN values

MPG(isnan(MPG))=0;
MPGS = sum(MPG(MPG > 1));
MPGMean = MPGS / 406;
MPG(MPG<=1)=MPGMean;


HP(isnan(HP))=0;
HPS = sum(HP(HP > 1));
HPMean = HPS / 406;
HP(HP<=1)=HPMean;

%%Replacing columns with NaN values with the arrays replacing NaN with the
%%mean
ImportT = removevars(ImportT,{'MPG','horsepower'});
ImportT = addvars(ImportT,MPG,'Before','Cylinders');
ImportT = addvars(ImportT,HP,'Before','weight');


%%Bubblesort the four variables
BubbleMPG = [MPG];
Entrys = length(BubbleMPG);
for j = 1:1:Entrys-1
    for i = 1:1:Entrys-1
        if BubbleMPG(i)>BubbleMPG(i+1)
            temp = BubbleMPG(i);
            BubbleMPG(i) = BubbleMPG(i+1);
            BubbleMPG(i+1) = temp;
        end
    end
end

BubbleHP = [HP];
Entrys = length(BubbleHP);
for j = 1:1:Entrys-1
    for i = 1:1:Entrys-1
        if BubbleHP(i)>BubbleHP(i+1)
            temp = BubbleHP(i);
            BubbleHP(i) = BubbleHP(i+1);
            BubbleHP(i+1) = temp;
        end
    end
end

BubbleA = [Acceleration];
Entrys = length(BubbleA);
for j = 1:1:Entrys-1
    for i = 1:1:Entrys-1
        if BubbleA(i)>BubbleA(i+1)
            temp = BubbleA(i);
            BubbleA(i) = BubbleA(i+1);
            BubbleA(i+1) = temp;
        end
    end
end

BubbleW = [Weight];
Entrys = length(BubbleW);
for j = 1:1:Entrys-1
    for i = 1:1:Entrys-1
        if BubbleW(i)>BubbleW(i+1)
            temp = BubbleW(i);
            BubbleW(i) = BubbleW(i+1);
            BubbleW(i+1) = temp;
        end
    end
end


%%Finding Mean/Median/Min/Max of variables needed
AccS = sum(Acceleration,'all');
AccMean = AccS / 406;
AccL = length(BubbleA);
AccV = (AccL+1)/2;
AccMedian = (BubbleA(floor(AccV))+BubbleA(ceil(AccV)))/2;
AccMin = BubbleA(1);
AccMax = BubbleA(406);

WS = sum(Weight,'all');
WMean = WS / 406;
WL = length(BubbleW);
WV = (WL+1)/2;
WMedian = (BubbleW(floor(WV))+BubbleW(ceil(WV)))/2;
WMin = BubbleW(1);
WMax = BubbleW(406);

HPL = length(BubbleHP);
HPV = (HPL+1)/2;
HPMedian = (BubbleHP(floor(HPV))+BubbleHP(ceil(HPV)))/2;
HPMin = BubbleHP(1);
HPMax = BubbleHP(406);

MPGL = length(BubbleMPG);
MPGV = (MPGL+1)/2;
MPGMedian = (BubbleMPG(floor(MPGV))+BubbleMPG(ceil(MPGV)))/2;
MPGMin = BubbleMPG(1);
MPGMax = BubbleMPG(406);

%%Finding Standard Deviation

SDMPG = [];
for i = 1:length(MPG);
    SDMPG = [SDMPG (MPG(i)- MPGMean)^2];
end

SDMPGs = sum(SDMPG);
SDMPGs = SDMPGs/(length(MPG)-1);
SDMPGs = sqrt(SDMPGs);

SDAcc = [];
for i = 1:length(Acceleration);
    SDAcc = [SDAcc (Acceleration(i)- AccMean)^2];
end

SDAccs = sum(SDAcc);
SDAccs = SDAccs/(length(Acceleration)-1);
SDAccs = sqrt(SDAccs);

SDHP = [];
for i = 1:length(HP);
    SDHP = [SDHP (HP(i)- HPMean)^2];
end

SDHPs = sum(SDHP);
SDHPs = SDHPs/(length(HP)-1);
SDHPs = sqrt(SDHPs);

SDW = [];
for i = 1:length(Weight);
    SDW = [SDW (Weight(i)- WMean)^2];
end

SDWs = sum(SDW);
SDWs = SDWs/(length(Weight)-1);
SDWs = sqrt(SDWs);

%%Putting needed values in table

Names = {'MPG';'Acceleration';'Horsepower';'Weight'};
ValueMeans = [MPGMean;AccMean;HPMean;WMean];
ValueMedians = [MPGMedian;AccMedian;HPMedian;WMedian];
ValueMin = [MPGMin;AccMin;HPMin;WMin];
ValueMax = [MPGMax;AccMax;HPMax;WMax];
StandardDiv = [SDMPGs;SDAccs;SDHPs;SDWs];

S3table = table(Names,ValueMeans,ValueMedians,ValueMin,ValueMax,StandardDiv);


%%Attempting to find Corrolation
%%CorMPG = MPG - MPGMean;
%%CorAcc = Acceleration - AccMean;
%%ak = CorMPG.*CorAcc;
%%cova = ak/(length(corMPG)-1);
%%corr = cova/(SDMPGs*SDAccs);

%%Randomizing car data file and splitting it into test and training data
ranrows = randperm(height(ImportT));
TrainingD = ImportT(ranrows(1:0.7*height(ImportT)), :);
TestD = ImportT(ranrows(0.7*height(ImportT)+1:end), :);


clearvars -except ImportT TestD TrainingD S3table Acceleration Weight Cyl MPG HP Disp





