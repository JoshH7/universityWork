Lin1 = Acceleration;
Lin2 = MPG;
format long;
varb1 = Lin1\Lin2;
%%varb1 is the regression coefficient

calc1 = varb1*Lin1;
scatter(Lin1,Lin2)
hold on
plot(Lin1,calc1)


xlabel('Acceleration')
ylabel('Miles per gallon')
title('Simple Linear Regression of Acceleration and MPG')
grid on
%%Create graph

Len = [ones(length(Lin1),1) Lin1];
b = Lin1\Lin2;