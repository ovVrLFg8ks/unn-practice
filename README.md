# unn-practice
Установка:
```
sudo make install
```
Запускаются по отдельности:
```
sudo systemctl start configuration
sudo systemctl start radio
sudo systemctl start fault
```
Остановка:
```
sudo systemctl stop configuration
sudo systemctl stop radio
sudo systemctl stop fault
```
Запуск на старте:
```
sudo systemctl [enable|disable] <daemon>
```
