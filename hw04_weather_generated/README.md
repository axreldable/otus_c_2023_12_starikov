# hw04_weather

Консольное приложение для получения текущей погоды через API [wttr.in](https://wttr.in) (формат JSON).

## Зависимости

- **libcurl** — HTTP-запросы
- **cJSON** — разбор JSON (исходники включены: `cJSON.c`, `cJSON.h`)

Установка libcurl (если нужна):

- macOS: `brew install curl` (часто уже есть)
- Linux: `sudo apt install libcurl4-openssl-dev` (Debian/Ubuntu) или `sudo dnf install libcurl-devel` (Fedora)

## Сборка

```bash
make
```

Компиляция с флагами: `-Wall -Wextra -Wpedantic -std=c11`.

## Запуск

```bash
./weather <город>
./weather Moscow
./weather "New York"
```

## Вывод

- Текстовое описание погоды (weather description)
- Направление и скорость ветра (например, SE 10 km/h)
- Температура (°C)

## Обработка ошибок

- Нет аргумента — вывод использования и выход с кодом 1
- Сетевые ошибки (таймаут, нет соединения) — сообщение и выход
- HTTP ≠ 200 (неверная локация, недоступность сервиса) — сообщение и выход
- Невалидный или неожиданный JSON — сообщение и выход
