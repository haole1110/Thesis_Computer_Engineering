import time
from function_AQI import post_data_to_Thingsboard, cal_aqi

while (1):
    response = post_data_to_Thingsboard(cal_aqi())
    time.sleep(1)