clc; clear; close all;

%% ----------------------
% System Parameters (Metric)
%% ----------------------
velocity = 5 * 0.3048;      % ft/s → m/s
filter_res = 0.10;          % 10% flow loss
target_fill_time = 30;      % minutes

pipe_sizes = [1 2 3 4] * 0.0254; % inches → meters

%% Pumps (with flow boost factor)
% Original GPM → L/s (1 GPM = 0.06309 L/s)
pumps = [
    struct('name',"Small",'lps',50*0.06309,'boost',2.0)
    struct('name',"Medium",'lps',200*0.06309,'boost',2.5)
    struct('name',"Large",'lps',500*0.06309,'boost',3.0)
    struct('name',"XL",'lps',1000*0.06309,'boost',3.5)
];

%% Level Sensors (reaction time same)
sensors = [
    struct('name',"Low",'level',0.1,'reaction',0.5)
    struct('name',"Middle",'level',0.5,'reaction',0.2)
    struct('name',"High",'level',0.9,'reaction',0.3)
];

%% Tanks (convert ft → m, gal → L)
tanks = [
    struct('capacity',2000*3.78541,'diameter',8*0.3048,'height',5.3*0.3048)
    struct('capacity',5000*3.78541,'diameter',10*0.3048,'height',8.5*0.3048)
    struct('capacity',10000*3.78541,'diameter',12*0.3048,'height',11.8*0.3048)
    struct('capacity',15000*3.78541,'diameter',14*0.3048,'height',13*0.3048)
    struct('capacity',20000*3.78541,'diameter',16*0.3048,'height',13.3*0.3048)
];

%% ----------------------
% Generate Dashboard Figures
%% ----------------------
for t = 1:length(tanks)
    tank = tanks(t);
    
    %% Required flow for target fill time (L/min)
    required_flow = tank.capacity / target_fill_time; 
    
    %% Determine recommended pump
    recommended_pump = "None";
    for p = 1:length(pumps)
        boosted_flow = pumps(p).lps * pumps(p).boost * 60; % L/s → L/min
        if boosted_flow >= required_flow
            recommended_pump = pumps(p).name;
            break;
        end
    end
    
    %% Create figure
    figure('Color','white','Position',[100 100 1600 1000])
    tiledlayout(2,2,'Padding','compact','TileSpacing','compact')
    
    %% ----------------------
    % Tank Layout + Sensors
    %% ----------------------
    nexttile
    hold on
    axis equal
    box on
    tank_width = 1; % m, visualization scale
    tank_height = 2; % m, visualization scale
    rectangle('Position',[0 0 tank_width tank_height],'LineWidth',3,'FaceColor',[0.9 0.95 1])
    
    for s = 1:length(sensors)
        y = sensors(s).level*tank_height;
        plot([0 tank_width],[y y],'--k','LineWidth',1.5)
        liters = tank.capacity * sensors(s).level;
        text(tank_width+0.05,y,...
            sprintf('%s\n%.0f%%\n%.0f L',sensors(s).name,sensors(s).level*100,liters),...
            'FontSize',10)
    end
    title(sprintf('Tank Layout + Sensors\nCapacity: %.0f L | Diameter: %.2f m | Height: %.2f m',...
        tank.capacity,tank.diameter,tank.height),'FontWeight','bold')
    xlim([-0.5 2])
    ylim([0 2.5])
    set(gca,'XTick',[])
    
    %% ----------------------
    % Pipe Fill Curves (all pipes)
    %% ----------------------
    nexttile
    hold on
    grid on
    box on
    colors = lines(length(pipe_sizes));
    
    for p = 1:length(pipe_sizes)
        pipe.diameter = pipe_sizes(p);
        pipe.velocity = velocity;
        pipe.resistance = 0;
        flow = pipeFlow(pipe);        % L/s
        fill_time = tank.capacity / (flow*60); % minutes
        time = linspace(0,fill_time,200);
        level = flow*60*time;         % L
        plot(time,level,'LineWidth',2,'Color',colors(p,:),...
            'DisplayName',sprintf('%.0f cm Pipe | %.1f L/s',pipe.diameter*100,flow))
    end
    xlabel('Time (minutes)')
    ylabel('Tank Level (L)')
    title('Pipe Size Fill Comparison','FontWeight','bold')
    legend('Location','northwest')
    ylim([0 tank.capacity])
    
    %% ----------------------
    % Pump Sizing
    %% ----------------------
    nexttile
    hold on
    grid on
    box on
    pump_flows = [pumps.lps]*60; % L/s → L/min
    bar(1:length(pumps),pump_flows,0.5)
    yline(required_flow,'r','LineWidth',2,'Label','Required Flow','LabelHorizontalAlignment','left')
    set(gca,'XTick',1:length(pumps),'XTickLabel',{pumps.name})
    ylabel('Flow Capacity (L/min)')
    title('Pump Sizing Analysis','FontWeight','bold')
    text(1,required_flow*0.9,sprintf('Recommended Pump: %s',recommended_pump),'FontWeight','bold','Color','b')
    
    %% ----------------------
    % Pipe Drain Curves (all pipes with filter)
    %% ----------------------
    nexttile
    hold on
    grid on
    box on
    colors = lines(length(pipe_sizes));
    
    for p = 1:length(pipe_sizes)
        pipe.diameter = pipe_sizes(p);
        pipe.velocity = velocity;
        pipe.resistance = filter_res;
        drain_flow = pipeFlow(pipe); % L/s
        drain_time = tank.capacity / (drain_flow*60); % minutes
        time = linspace(0,drain_time,200);
        level = tank.capacity - drain_flow*60*time; % L
        plot(time,level,'LineWidth',2,'Color',colors(p,:),...
            'DisplayName',sprintf('%.0f cm Pipe | %.1f L/s',pipe.diameter*100,drain_flow))
    end
    xlabel('Time (minutes)')
    ylabel('Tank Level (L)')
    title('Filtered Drain Curves','FontWeight','bold')
    legend('Location','northeast')
    ylim([0 tank.capacity])
    
    %% ----------------------
    % Save figure
    %% ----------------------
    imgfile = sprintf('TankDashboard_%dL.png',round(tank.capacity));
    exportgraphics(gcf,imgfile,'Resolution',300)
end

disp('Full dashboards generated for all tanks, pipes, pumps, sensors!')

%% ----------------------
% Pipe Flow Function (Metric)
%% ----------------------
function flow = pipeFlow(pipe)
PI = 3.141592653589793;
D_m = pipe.diameter;       % meters
area = PI * D_m^2 / 4;     % m²
raw = area * pipe.velocity * 1000; % m³/s → L/s
flow = raw * (1 - pipe.resistance);
end