import math
import csv
from datetime import datetime, timedelta

I = [0, 50, 100, 150, 200, 300, 400, 500]
o3 = [0, 160, 200, 300, 400, 800, 1000, 1200]
co = [0, 10000, 30000, 45000, 60000, 90000, 120000, 150000]
so2 = [0, 125, 350, 550, 800, 1600, 2100, 2630]
no2 = [0, 100, 200, 700, 1200, 2350, 3100, 3850]
pm10 = [0, 50, 150, 250, 350, 420, 500, 600]
pm25 = [0, 25, 50, 80, 150, 250, 350, 500]
data_file = "data_with_aqi.csv"
output_file = "data_with_aqi.csv"
pm10_col = 7
pm25_col = 8
co_col = 9
co2_col = 10  # Not used for AQI calculation
so2_col = 11
no2_col = 12
o3_col = 13

# list_val_pm10 = [100, 200, 300, 400, 350, 450, 150, 120, 220, 340, 110, 360]
# list_val_pm25 = [20.6, 19.6, 22.4, 20.3, 16.5, 19.0, 16.5, 19.5, 23.5, 20.5, 24.7, 26.9]


def cal_nowcast(arr):
    numerator = 0
    denominator = 0
    nowcast = 0
    temp = 0
    if min(arr)/max(arr) <= 1/2:
        w = 0.5
        for i in range(len(arr)):
            nowcast = nowcast + (pow(0.5, i+1) * arr[i])
    else:
        w = round(min(arr)/max(arr), 2)
        for i in range(len(arr)):
            numerator = numerator + (pow(w, i) * arr[i])
            denominator = denominator + pow(w, i)
        nowcast = numerator / denominator
    return round(nowcast, 1)


def cal_aqi(list_val_pm10, list_val_pm25, val_co, val_so2, val_no2, val_o3):


    # val_temp = get_data_Thingsboard('temperature')
    # val_humi = get_data_Thingsboard('humidity')
    # val_co2 = get_data_Thingsboard('co2')
    # val_o3 = get_data_Thingsboard('o3')
    # val_no2 = get_data_Thingsboard('no2')
    # val_co = get_data_Thingsboard('co')
    # val_so2 = get_data_Thingsboard('so2')
    # list_val_pm10 = get_data_Thingsboard('pm10')
    # list_val_pm25 = get_data_Thingsboard('pm2-5')

    aqi_result = -1
    index_o3 = 0
    index_no2 = 0
    index_co = 0
    index_so2 = 0
    index_pm10 = 0
    index_pm25 = 0

    while 2.14*val_o3*1000 < o3[index_o3]:
        index_o3 = index_o3 + 1
    aqi_o3 = ((I[index_o3 + 1] - I[index_o3]) / (o3[index_o3 + 1] - o3[index_o3])) * (val_o3 - o3[index_o3]) + I[index_o3]
    if aqi_result < aqi_o3:
        aqi_result = aqi_o3

    while 2.05*val_no2*1000 < no2[index_no2]:
        index_no2 = index_no2 + 1
    aqi_no2 = ((I[index_no2 + 1] - I[index_no2]) / (no2[index_no2 + 1] - no2[index_no2])) * (val_no2 - no2[index_no2]) + I[index_no2]
    if aqi_result < aqi_no2:
        aqi_result = aqi_no2

    while 1.25*val_co*1000 < co[index_co]:
        index_co = index_co + 1
    aqi_co = ((I[index_co + 1] - I[index_co]) / (co[index_co + 1] - co[index_co])) * (val_co - co[index_co]) + I[index_co]
    if aqi_result < aqi_co:
        aqi_result = aqi_co

    while 2.86*val_so2*1000 < so2[index_so2]:
        index_so2 = index_so2 + 1
    aqi_so2 = ((I[index_so2 + 1] - I[index_so2]) / (so2[index_so2 + 1] - so2[index_so2])) * (val_so2 - so2[index_so2]) + I[index_so2]
    if aqi_result < aqi_so2:
        aqi_result = aqi_so2

    while list_val_pm10[0] < pm10[index_pm10]:
        index_pm10 = index_pm10 + 1
    aqi_pm10 = ((I[index_pm10 + 1] - I[index_pm10]) / (pm10[index_pm10 + 1] - pm10[index_pm10])) * (cal_nowcast(list_val_pm10) - pm10[index_pm10]) + I[index_pm10]
    if aqi_result < aqi_pm10:
        aqi_result = aqi_pm10

    while list_val_pm25[0] < pm25[index_pm25]:
        index_pm25 = index_pm25 + 1
    aqi_pm25 = ((I[index_pm25 + 1] - I[index_pm25]) / (pm25[index_pm25 + 1] - pm25[index_pm25])) * (cal_nowcast(list_val_pm25) - pm25[index_pm25]) + I[index_pm25]
    if aqi_result < aqi_pm25:
        aqi_result = aqi_pm25

    return round(aqi_result + 0.5)


def calculate_aqi(i):
  # Read data from CSV file
  with open(data_file, 'r') as csvfile:
    reader = csv.reader(csvfile)
    data = list(reader)

  pm10_arr = []
  pm25_arr = []

  if i<13:
      for j in range(i, 1, -1):
          pm10_arr.append(float(data[j][pm10_col]))
          pm25_arr.append(float(data[j][pm25_col]))
  else:
      for j in range(i, i-12*6, -6):
          pm10_arr.append(float(data[j][pm10_col]))
          pm25_arr.append(float(data[j][pm25_col]))

  # Extract pollutant values
  co = float(data[i][co_col])
  so2 = float(data[i][so2_col])
  no2 = float(data[i][no2_col])
  o3 = float(data[i][o3_col])

# Calculate AQI using the provided function
  aqi = cal_aqi(pm10_arr, pm25_arr, co, so2, no2, o3)

# Add AQI value to the current row
  data[i].append(aqi)

  return aqi


#     # Add a new column for AQI
#     data[0].append("AQI")  # Add header for AQI column in the first row
#
#
# # Write data with AQI to a new CSV file
# with open(output_file, 'w', newline='') as csvfile:
#     writer = csv.writer(csvfile)
#     for row in range(2, 8867):
#         calculate_aqi(row)
#     writer.writerows(data)

