% Please, load datacd.mat before run the script.

% Resolution of the sensor
xlength = 640;
ylength = 480;

% Frame rate (Hz) and frame time (us) of the sensor
frameRate = 25;
frameTime = floor(1/frameRate * 1e6);

% Initialize variables to create the image and perform the loop
colourArray = 0.5*ones(ylength, xlength); % An array between 0 and 1, which represent the colour of the pixel
frameId = 1;
lastframeTime = 0;

% Updating indexs to convert them according to MATLAB 
map.x = map.x + 1;
map.y = map.y + 1;
j = 1;
k = 1;
% Build a figure for movie displaye
figure();
image(colourArray); % An image is created. It is an array between 0 and 1, which represent the colour of the pixel
colormap gray; % The image will be ploted in gray scale
colormap_size = size(colormap, 1); % Size of the gray colormap (64), 64 will be black and 0 will be white

% Storing the first empty frame
mov(frameId) = getframe(gcf); % The initial frame is obtained
frameId = frameId+1; % The current frame is updated

% Vectors to store the time, x position and y position of the dots
tblack = 1:length(map.ts);
xblack = 1:length(map.x);
yblack = 1:length(map.y);

twhite = 1:length(map.ts);
xwhite = 1:length(map.x);
ywhite = 1:length(map.y);

% Go through all events in file
for i=1:length(map.ts)
    % Get the time of the current event
    currentframeTime = map.ts(i);
    
    % Add event to state
    if map.p(i) == 1
        % White
        colourArray(map.y(i), map.x(i)) = 1;
        xwhite(k) = map.x(i);
        ywhite(k) = map.y(i);
        twhite(k) = lastframeTime + frameTime;
        k = k+1;
    else 
        % Black
        colourArray(map.y(i), map.x(i)) = 0;
        xblack(j) = map.x(i);
        yblack(j) = map.y(i);
        tblack(j) = lastframeTime + frameTime;
        j = j+1;
    end
       
    % Check if images should be generated
    if(currentframeTime > lastframeTime + frameTime)
        % Update frame
         lastframeTime = lastframeTime + frameTime;
        % Display array colour
        image(colormap_size*colourArray);
        % The current image is stored in the actual frame
        mov(frameId) = getframe();
        frameId = frameId+1;
        %Reset pixels
        colourArray = 0.5*ones(ylength, xlength);
    end
end

% Ploting data over time
figure();
plot3(xblack,tblack,yblack,'r*');
hold on;
plot3(xwhite,twhite,ywhite,'y*');
grid;
axis([1 xlength 0 lastframeTime 1 ylength]);
set(gca,'color',[1 1 1])
set(gcf,'color',[0.4 0.4 0.4]);
xlabel('X axis (pixel)');
ylabel('Time (us)');
zlabel('Y axis (pixel)');

