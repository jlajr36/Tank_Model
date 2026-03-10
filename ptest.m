data = [];

for N = 1:50
    newValue = rand();
    data = [data newValue];
end

plot(data);
xlabel('Iteration');
ylabel('Value');