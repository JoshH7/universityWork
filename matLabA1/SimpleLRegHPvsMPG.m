Lin3 = HP;
Lin4 = MPG;
format long;
varb2 = Lin3\Lin4;
%%varb1 is the regression coefficient

calc2 = varb2*Lin3;
scatter(Lin3,Lin4)
hold on
plot(Lin3,calc2)


xlabel('Horsepower')
ylabel('Miles per gallon')
title('Simple Linear Regression of Horsepower and MPG')
grid on
%%Create graph

Len = [ones(length(Lin3),1) Lin3];
b2 = Lin3\Lin4;