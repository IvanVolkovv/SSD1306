### Общее описание:
Небольшая библиотека для отрисовки простых двумерных графиков, а также некоторых виджетов. Используется для дисплеев на контроллере - SSD1306. 
Библиотека написана под микроконтроллеры серии STM, связь с дисплеем осуществляется по интерфейсу I2C.
### Подключаемые файлы:
Для работы с библиотекой необходимо скорректировать низкоуровневые ф-ии управления I2C интерфейсом под вашу конкретную модель микроконтроллера (МК). Ф-ии управления I2C находятся в файле - i2c_init.с. 
Если используется МК STM32F103, то ничего корректировать не нужно, т.к. изначально всё тестилось на данном МК. Файл - Fonts_SSD1306.с содержит массивы для отображения символов. Базовым файлом библиотеки является - Graphs_SSD1306.с, который содержит основные ф-ии управления дисплеем (очистка, выставить курсор, вывести строку текста и т.д.). Специализированные файлы - Graphs_SSD1306.с и Widgets_SSD1306.с соответственно реализуют отрисовку двумерных графиков или виджетов. Зависимость файлов схематично показана на рисунке ниже. 
<p align="center">
  <img src="https://github.com/user-attachments/assets/fdfa012a-da86-4a37-ada2-66d78cc3f5c1">
</p>

### Настройка двумерного графика:
Параметры графика настраиваются перед началом работы установкой выбранных свойств в поля экземпляра структуры типа - PropertiesGraphXY. 
Далее ниже представлен вид графика в зависимости от выбранных свойств.

Тип расположения осей графика: .type = XY_TYPE_1 или .type = XY_TYPE_2
<p align="center">
  <img src="https://github.com/user-attachments/assets/bfc33591-f67e-4d82-aa0c-bf64f3a88063">
</p>

Отображение максимального значения для осей: .numbers = NUM_On или .numbers = NUM_Off
<p align="center">
  <img src="https://github.com/user-attachments/assets/06c07e6a-1846-4018-8570-7987a8c60431">
</p> 
