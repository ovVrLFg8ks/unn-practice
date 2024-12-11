# unn-practice
Установка:
```
sudo make install
```
Запускаются по отдельности:
```
sudo systemctl start radio
sudo systemctl start configuration
sudo systemctl start fault
```
```
sudo systemctl start radio && sudo systemctl start configuration && sudo systemctl start fault
```

Остановка:
```
sudo systemctl stop configuration
sudo systemctl stop radio
sudo systemctl stop fault
```
```
sudo systemctl stop configuration && sudo systemctl stop radio && sudo systemctl stop fault
```
Запуск на старте:
```
sudo systemctl [enable|disable] <daemon>
```
---
Достойны упоминания:
```
tail -f /var/log/syslog
```
```
sudo systemctl --type=service --state=active
```
```
sudo systemctl daemon-reload
```

... ну типа сейчас дæмŏн с SharMem клиентом закрывается МАКСИМУМ за 14 сек
это в теории, на практике – меньше
