
try
{
    $env:DOTS_LOG_LEVEL = 5
    $services = $(
        Start-Process .\_deps\dots-cpp-build\bin\dotsd\dotsd.exe -PassThru -NoNewWindow;
        Start-Process .\services\sensor-service\sensor-service.exe -ArgumentList '"Lower Hallway" "0.2"' -PassThru -NoNewWindow;
        Start-Process .\services\sensor-service\sensor-service.exe -ArgumentList '"Upper Hallway" "0.4"' -PassThru -NoNewWindow;
        Start-Process .\services\sensor-service\sensor-service.exe -ArgumentList '"Staircase" "0.6"' -PassThru -NoNewWindow;
        Start-Process .\services\sensor-service\sensor-service.exe -ArgumentList '"Office" "0.2"' -PassThru -NoNewWindow;
        Start-Process .\services\sensor-service\sensor-service.exe -ArgumentList '"Basement" "0.8"' -PassThru -NoNewWindow;
        Start-Process .\services\alarm-service\alarm-service.exe -PassThru -NoNewWindow;
        Start-Process .\services\hmi-service\hmi-service.exe -PassThru -NoNewWindow;
    )
    $services | Wait-Process
}
finally
{
    $services | Wait-Process
    Remove-Item env:DOTS_LOG_LEVEL
}
