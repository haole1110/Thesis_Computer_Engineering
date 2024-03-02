import math
import requests
from datetime import datetime, timedelta

I = [0, 50, 100, 150, 200, 300, 400, 500]
o3 = [0, 160, 200, 300, 400, 800, 1000, 1200]
co = [0, 10000, 30000, 45000, 60000, 90000, 120000, 150000]
so2 = [0, 125, 350, 550, 800, 1600, 2100, 2630]
no2 = [0, 100, 200, 700, 1200, 2350, 3100, 3850]
pm10 = [0, 50, 150, 250, 350, 420, 500, 600]
pm25 = [0, 25, 50, 80, 150, 250, 350, 500]

# list_val_pm10 = [100, 200, 300, 400, 350, 450, 150, 120, 220, 340, 110, 360]
# list_val_pm25 = [20.6, 19.6, 22.4, 20.3, 16.5, 19.0, 16.5, 19.5, 23.5, 20.5, 24.7, 26.9]

def cal_nowcast(arr):
    numerator = 0
    denominator = 0
    nowcast = 0
    temp = 0
    if min(arr)/max(arr) <= 1/2:
        w = 0.5
        for i in range(12):
            nowcast = nowcast + (pow(0.5, i+1) * arr[i])
    else:
        w = round(min(arr)/max(arr), 2)
        for i in range(12):
            numerator = numerator + (pow(w, i) * arr[i])
            denominator = denominator + pow(w, i)
        nowcast = numerator / denominator
    return round(nowcast, 1)

def get_data_Thingsboard(key):
    token = "Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJsZWR1eWhhbzQ0QGdtYWlsLmNvbSIsInVzZXJJZCI6IjJmZmIzNTYwLTk1ODctMTFlZS1iODFhLTFiZGNkY2JkOGRkMCIsInNjb3BlcyI6WyJURU5BTlRfQURNSU4iXSwic2Vzc2lvbklkIjoiMjA1NDAyZWItNGQ3NC00Zjk1LTg5Y2MtNGY5YTE1MTExYzgyIiwiaXNzIjoidGhpbmdzYm9hcmQuaW8iLCJpYXQiOjE3MDg3NTE3NTksImV4cCI6MTcxMDU1MTc1OSwiZmlyc3ROYW1lIjoiSGFvIiwibGFzdE5hbWUiOiJMZSIsImVuYWJsZWQiOnRydWUsInByaXZhY3lQb2xpY3lBY2NlcHRlZCI6dHJ1ZSwiaXNQdWJsaWMiOmZhbHNlLCJ0ZW5hbnRJZCI6IjJkM2I3OTIwLTk1ODctMTFlZS1iODFhLTFiZGNkY2JkOGRkMCIsImN1c3RvbWVySWQiOiIxMzgxNDAwMC0xZGQyLTExYjItODA4MC04MDgwODA4MDgwODAifQ.DcMJie02CFSkP0AdzQ2do3uby29o_jbD2nLQcSHXYv6xTpZVoXoB6CjmGZG3FRf6PeQYuLqCMahKv92t710hKA"
    DeviceID = "2ad949b0-aa21-11ee-bdf7-d513ee05d751"
    headers = {'Content-Type': 'application/json', 'X-Authorization': token}
    if key != 'pm2-5' and key != 'pm10':
        url = f'http://demo.thingsboard.io/api/plugins/telemetry/DEVICE/{DeviceID}/values/timeseries?'
        response = requests.get(url, headers=headers)
        datajson = response.json()
        return float(datajson[key][0]['value'])
    else:
        current_time_local = datetime.now()
        milliseconds_local = int(current_time_local.timestamp() * 1000)
        time_12_hours_ago = current_time_local - timedelta(hours=1200)
        time_12_hours_ago_milliseconds = int(time_12_hours_ago.timestamp() * 1000)
        url = f'http://demo.thingsboard.io/api/plugins/telemetry/DEVICE/{DeviceID}/values/timeseries?'
        para = {
            "keys": key,
            "startTs": time_12_hours_ago_milliseconds,
            "endTs": milliseconds_local,
            "limit": 12
        }
        response = requests.get(url, headers=headers, params=para)
        datajson = response.json()
        value_array = []

        for item in datajson[key]:
            value = float(item['value'])
            value_array.append(value)
        return value_array

def cal_aqi():
    aqi_result = -1

    # val_temp = get_data_Thingsboard('temperature')
    # val_humi = get_data_Thingsboard('humidity')
    # val_co2 = get_data_Thingsboard('co2')
    val_o3 = get_data_Thingsboard('o3')
    val_no2 = get_data_Thingsboard('no2')
    val_co = get_data_Thingsboard('co')
    val_so2 = get_data_Thingsboard('so2')
    list_val_pm10 = get_data_Thingsboard('pm10')
    list_val_pm25 = get_data_Thingsboard('pm2-5')

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

def post_data_to_Thingsboard(aqi):
    token = "Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJsZWR1eWhhbzQ0QGdtYWlsLmNvbSIsInVzZXJJZCI6IjJmZmIzNTYwLTk1ODctMTFlZS1iODFhLTFiZGNkY2JkOGRkMCIsInNjb3BlcyI6WyJURU5BTlRfQURNSU4iXSwic2Vzc2lvbklkIjoiMjA1NDAyZWItNGQ3NC00Zjk1LTg5Y2MtNGY5YTE1MTExYzgyIiwiaXNzIjoidGhpbmdzYm9hcmQuaW8iLCJpYXQiOjE3MDg3NTE3NTksImV4cCI6MTcxMDU1MTc1OSwiZmlyc3ROYW1lIjoiSGFvIiwibGFzdE5hbWUiOiJMZSIsImVuYWJsZWQiOnRydWUsInByaXZhY3lQb2xpY3lBY2NlcHRlZCI6dHJ1ZSwiaXNQdWJsaWMiOmZhbHNlLCJ0ZW5hbnRJZCI6IjJkM2I3OTIwLTk1ODctMTFlZS1iODFhLTFiZGNkY2JkOGRkMCIsImN1c3RvbWVySWQiOiIxMzgxNDAwMC0xZGQyLTExYjItODA4MC04MDgwODA4MDgwODAifQ.DcMJie02CFSkP0AdzQ2do3uby29o_jbD2nLQcSHXYv6xTpZVoXoB6CjmGZG3FRf6PeQYuLqCMahKv92t710hKA"
    DeviceID = "2ad949b0-aa21-11ee-bdf7-d513ee05d751"
    headers = {'Content-Type': 'application/json', 'X-Authorization': token}

    url = f'http://demo.thingsboard.io/api/plugins/telemetry/DEVICE/{DeviceID}/timeseries/ANY?scope=ANY'

    # Dữ liệu cần gửi lên Thingsboard
    data = {
        "aqi": aqi
    }
    # Gửi yêu cầu POST lên Thingsboard
    response = requests.post(url, headers=headers, json=data)
