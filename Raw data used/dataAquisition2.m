serialPort = "COM7"; 
baudRate = 9600;
outputFile = 'PWM_85.csv'; % aici am salvat pe rand PWM_85, PWM_128, PWM_255


s = serialport(serialPort, baudRate);
fopen(s);
disp("Reading data...");

fileID = fopen(outputFile, 'w');
fprintf(fileID, "Speed Left,Speed Right");

while true
    data = readline(s); % Read line from Arduino
    fprintf("%s\n", data); % Display data in MATLAB
    fprintf(fileID, "%s\n", data); % Save data to file

    if contains(data, "END") % Stop condition (optional)
        break;
    end
end

% Close resources
fclose(fileID);
delete(s);
disp("PWM_85.csv");

