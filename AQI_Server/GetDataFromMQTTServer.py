import paho.mqtt.client as mqtt
import time
import json
import requests
from datetime import datetime
from function_AQI import post_data_to_Thingsboard
import pandas as pd
import csv
import pytz
from Function_AQI_Thang import calculate_aqi


MQTT_SERVER = "mqttserver.tk"
MQTT_PORT = 1883
MQTT_USERNAME = "innovation"
MQTT_PASSWORD = "Innovation_RgPQAZoA5N"

MQTT_TOPIC = "/innovation/airmonitoring/WSNs"

def add_AQI_last_row():
    filename = 'data_with_aqi.csv'
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        rows = list(reader)

    # Ghi giá trị vào cột ở dòng cuối
    rows[-1][-1] = calculate_aqi(len(rows) - 1)

    with open(filename, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerows(rows)
    return rows[-1][-1]
def add_new_data(data):

    # Lấy giá trị từ biến current_time và timestamp
    current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    timestamp = datetime.strptime(current_time, "%Y-%m-%d %H:%M:%S").timestamp()

    # Định dạng ngày tháng ban đầu
    original_format = '%Y-%m-%d %H:%M:%S'

    # Định dạng ngày tháng mới
    new_format = '%d/%m/%Y %H:%M:%S'

    # Chuyển đổi ngày tháng
    parsed_date = datetime.strptime(current_time, original_format)
    converted_date = datetime.strftime(parsed_date, new_format)

    # Thêm giá trị timestamp và current_time vào dữ liệu
    data['Timestamp'] = timestamp
    data['Date'] = converted_date

    # Đường dẫn đến file CSV
    filename = 'data_with_aqi.csv'


    # Ghi dữ liệu vào file CSV
    with open(filename, 'a', newline='') as file:
        writer = csv.writer(file)

        # Kiểm tra nếu file CSV rỗng, ghi header
        if file.tell() == 0:
            header = ['Timestamp', 'Date', 'temp_0001', 'humi_0001', 'illuminance_0001', 'atmosphere_0001',
                      'noise_0001', 'pm10_0001', 'pm2.5_0001', 'CO_0001', 'CO2_0001', 'SO2_0001', 'NO2_0001',
                      'O3_0001', 'temp_0002', 'humi_0002', 'ph_0002', 'EC_0002', 'Nito_0002', 'Photpho_0002',
                      'Kali_0002', 'vol_0001', 'vol_0002', 'power_0001', 'AQI']
            writer.writerow(header)

        row = [timestamp, converted_date, data.get('temp_0001'), data.get('humi_0001'),
               data.get('illuminance_0001'), data.get('atmosphere_0001'), data.get('noise_0001'),
               data.get('pm10_0001'), data.get('pm2.5_0001'), data.get('CO_0001'), data.get('CO2_0001'),
               data.get('SO2_0001'), data.get('NO2_0001'), data.get('O3_0001'), data.get('temp_0002'),
               data.get('humi_0002'), data.get('ph_0002'), data.get('EC_0002'), data.get('Nito_0002'),
               data.get('Photpho_0002'), data.get('Kali_0002'), data.get('vol_0001'), data.get('vol_0002'),
               data.get('power_0001')]
        converted_data = [str(int(row[0])), row[1], row[2].replace('.', ','), row[3].replace('.', ','), row[4],
                          row[5].replace('.', ','), row[6].replace('.', ','), int(float(row[7])),
                          int(float(row[8])), int(float(row[9])), int(float(row[10])), int(float(row[11])),
                          int(float(row[12])), int(float(row[13])), row[14].replace('.', ','),
                          row[15].replace('.', ','), row[16].replace('.', ','), int(float(row[17])),
                          int(float(row[18])), int(float(row[19])), int(float(row[20])), row[21].replace('.', ','),
                          row[22].replace('.', ','), row[23].replace('.', ','), 1]
        csv_row = ','.join(['"{}"'.format(str(item)) for item in converted_data])
        writer.writerow(converted_data)

def mqtt_connected(client, userdata, flags, rc):
    print("Connected succesfully!!")
    client.subscribe(MQTT_TOPIC)


def mqtt_subscribed(client, userdata, mid, granted_qos):
    print("\nSubscribed to Topic!!!")


def mqtt_recv_message(client, userdata, message):
    payload_str = message.payload.decode('utf-8')
    payload_str = payload_str.replace("'", '"')
    #print(f"{payload_str}\n")
    data = json.loads(payload_str)
    sensors = data['sensors']
    json_data = json.dumps({item["id"]: item["value"] for item in sensors})
    json_data = json.loads(json_data)

    add_new_data(json_data)
    aqi = add_AQI_last_row()

    json_data["aqi"] = aqi
    json_data = json.dumps(json_data)
    json_data = json.loads(json_data)

    print(json_data)
    response = post_data_to_Thingsboard(json_data)


mqttClient = mqtt.Client()
mqttClient.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)
mqttClient.connect(MQTT_SERVER, int(MQTT_PORT), 60)

mqttClient.on_connect = mqtt_connected
mqttClient.on_subscribe = mqtt_subscribed
mqttClient.on_message = mqtt_recv_message

mqttClient.loop_start()
