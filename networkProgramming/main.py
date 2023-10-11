import pandas as pd

# Khởi tạo DataFrame
data = {'Col1': [1, 2, 3],
        'Col2': [4, 5, 6],
        'Col3': [7, 8, 9]}

df = pd.DataFrame(data)

# Tính tổng của 3 cột và lưu vào cột cuối
df['Tong'] = df['Col1'] + df['Col2'] + df['Col3']

# In ra DataFrame sau khi tính tổng
print(df)
