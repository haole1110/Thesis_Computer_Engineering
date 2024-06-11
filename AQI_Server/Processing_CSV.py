import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns


# Đường dẫn tới tệp CSV gốc
input_file = 'data_with_aqi.csv'

# Đường dẫn tới tệp CSV kết quả sau khi xóa cột
output_file = 'final.csv'

# Đọc dữ liệu từ tệp CSV gốc
df = pd.read_csv(input_file)

# Xóa cột bằng tên cột
column_to_delete = 'AQI'
df.drop(column_to_delete, axis=1, inplace=True)

# Lưu lại dữ liệu đã xóa cột vào tệp kết quả
df.to_csv(output_file, index=False)