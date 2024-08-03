import pandas as pd

df = pd.read_csv("files/data.csv")
df['date'] = pd.to_datetime(df['Дата'], format="%d.%m.%y %H")
start_date_input = "02.08.24 22"
end_date_input = "03.08.24 05"
start_date = pd.to_datetime(start_date_input, format="%d.%m.%y %H")
end_date = pd.to_datetime(end_date_input, format="%d.%m.%y %H")


min_date = df['date'].min()
max_date = df['date'].max()

# Проверяем начальную дату
if start_date < min_date:
    nearest_start = min_date
elif start_date > max_date:
    nearest_start = max_date
else:
    nearest_start = start_date

# Проверяем конечную дату
if end_date < min_date:
    nearest_end = min_date
elif end_date > max_date:
    nearest_end = max_date
else:
    nearest_end = end_date


filtered_df = df[(df['date'] >= nearest_start) & (df['date'] <= nearest_end)]

print(filtered_df)

