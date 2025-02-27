import csv
import random
from faker import Faker

def generate_csv(num_records, filename):
    fake = Faker()
    
    # Заголовки столбцов
    headers = ["Name", "Rank", "CompanyNumber", "Age"]
    
    # Генерация данных
    data = []
    for _ in range(num_records):
        first_name = fake.first_name()
        last_name = fake.last_name()    
        name = f"{first_name} {last_name}"
        rank = random.randint(1, 40)
        company_number = random.randint(1, 10)
        age = random.randint(18, 65)
        data.append([name, rank, company_number, age])
    
    # Запись данных в CSV-файл
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(headers)
        writer.writerows(data)

    print(f"Файл {filename} успешно создан с {num_records} записями.")

# Пример использования
num_records = int(input("Введите кол-во записей: "))  # Количество записей
filename = f"data_{num_records}.csv"
generate_csv(num_records, filename)