clc; clear; close all;

%% ----------------------
% System Parameters
%% ----------------------
velocity = 5;          % ft/s
filter_res = 0.10;     % 10% flow loss
target_fill_time = 30; % minutes

pipe_sizes = [1 2 3 4]; % inches

%% Pumps (with flow boost factor)
pumps = [
    struct('name',"Small",'gpm',50,'boost',2.0)
    struct('name',"Medium",'gpm',200,'boost',2.5)
    struct('name',"Large",'gpm',500,'boost',3.0)
    struct('name',"XL",'gpm',1000,'boost',3.5)
];

%% Level Sensors (with reaction times)
sensors = [
    struct('name',"Low",'level',0.1,'reaction',0.5)
    struct('name',"Middle",'level',0.5,'reaction',0.2)
    struct('name',"High",'level',0.9,'reaction',0.3)
];

%% Tanks (all 5)
tanks = [
    struct('capacity',2000,'diameter',8,'height',5.3)
    struct('capacity',5000,'diameter',10,'height',8.5)
    struct('capacity',10000,'diameter',12,'height',11.8)
    struct('capacity',15000,'diameter',14,'height',13)
    struct('capacity',20000,'diameter',16,'height',13.3)
];

%% ----------------------
% Generate Dashboard Figures
%% ----------------------
for t = 1:length(tanks)
    tank = tanks(t);
    
    %% Required flow for target fill time
    required_flow = tank.capacity / target_fill_time;
    
    % Determine recommended pump
    recommended_pump = "None";
    for p = 1:length(pumps)
        boosted_flow = min(pumps(p).gpm * pumps(p).boost, pumps(p).gpm);
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
    tank_width = 4;
    tank_height = 10;
    rectangle('Position',[0 0 tank_width tank_height],'LineWidth',3,'FaceColor',[0.9 0.95 1])
    
    for s = 1:length(sensors)
        y = sensors(s).level*tank_height;
        plot([0 tank_width],[y y],'--k','LineWidth',1.5)
        gallons = tank.capacity * sensors(s).level;
        text(tank_width+0.2,y,...
            sprintf('%s\n%.0f%%\n%.0f gal',sensors(s).name,sensors(s).level*100,gallons),...
            'FontSize',10)
    end
    title(sprintf('Tank Layout + Sensors\nCapacity: %d gal | Diameter: %.1f ft | Height: %.1f ft',...
        tank.capacity,tank.diameter,tank.height),'FontWeight','bold')
    xlim([-1 6])
    ylim([0 12])
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
        flow = pipeFlow(pipe);        
        fill_time = tank.capacity / flow;
        time = linspace(0,fill_time,200);
        level = flow*time;
        plot(time,level,'LineWidth',2,'Color',colors(p,:),...
            'DisplayName',sprintf('%d" Pipe | %.0f GPM',pipe.diameter,flow))
    end
    xlabel('Time (minutes)')
    ylabel('Tank Level (gal)')
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
    pump_flows = [pumps.gpm];
    bar(1:length(pumps),pump_flows,0.5)
    yline(required_flow,'r','LineWidth',2,'Label','Required Flow','LabelHorizontalAlignment','left')
    set(gca,'XTick',1:length(pumps),'XTickLabel',{pumps.name})
    ylabel('Flow Capacity (GPM)')
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
        drain_flow = pipeFlow(pipe);
        drain_time = tank.capacity / drain_flow;
        time = linspace(0,drain_time,200);
        level = tank.capacity - drain_flow*time;
        plot(time,level,'LineWidth',2,'Color',colors(p,:),...
            'DisplayName',sprintf('%d" Pipe | %.0f GPM',pipe.diameter,drain_flow))
    end
    xlabel('Time (minutes)')
    ylabel('Tank Level (gal)')
    title('Filtered Drain Curves','FontWeight','bold')
    legend('Location','northeast')
    ylim([0 tank.capacity])
    
    %% ----------------------
    % Save figure
    %% ----------------------
    imgfile = sprintf('TankDashboard_%d.png',tank.capacity);
    exportgraphics(gcf,imgfile,'Resolution',300)
end

disp('Full dashboards generated for all tanks, pipes, pumps, sensors!')

%% ----------------------
% Pipe Flow Function
%% ----------------------
function flow = pipeFlow(pipe)
PI = 3.141592653589793;
D_ft = pipe.diameter / 12;
area = PI * D_ft^2 / 4;
raw = area * pipe.velocity * 7.48052 * 60; % ft³/s → GPM
flow = raw * (1 - pipe.resistance);
end