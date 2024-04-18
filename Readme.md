# Case Engine
<!-- описание проекта -->
Программное обеспечение для разработки компьютерных игр
 
<!-- ссылка на Web сайт -->
![WEB](https://caseengine.ru)

<!-- путь к логотипу -->
![Logotype](source/Resources/Icons/case-engine_logo.bmp)

<!-- установка -->
## Установка (Windows)
У вас должен быть установлен [DirectX SDK](https://www.microsoft.com/ru-ru/download/details.aspx?id=6812)

1. Распакуйте ZIP архив

``` Case-Engine.zip ```

2. Скачайте Archive.zip по ссылке: [Archive](https://drive.google.com/file/d/1r2QZwoNhWJCmMZWtjFSNJ7PQ6OCi8cY9/view?usp=sharing)

3. Распакуйте ZIP архив

``` Archive.zip ```

4. Переместите папку ```Resources``` в папку ```source```

5. Оставьте папку ```includes``` в главной папке

6. Переместите папку ```Dlls``` в папку ```source```

<!-- сборка проекта -->
## Сборка (Windows, x64 Debug)

1. Запустите BAT файл

``` build.bat ```

<!-- ориентация по проекту -->
## Ориентация по проекту
| Папка    | Описание                                                        |
|----------|-----------------------------------------------------------------|
| CMake	   | Хранение .cmake файлов                                          |
| includes | Хранение библиотек                                              |
| out      | Хранение файлов полученных после сборки                         |
| source   | Хранение исходников                                             |
| source/Core     | Исходные файлы ядра                                      |
| source/DLLs     | Хранение динамических библиотек                          |
| source/Editor   | Исходные файлы редактора                                 |
| source/Graphics | Исходные файлы графического движка                       |
| source/Math     | Математические утилиты                                   |
| source/Rendering| Исходные файлы объектов движка                           |
| source/Resources| Хранение ресурсов                                        |
| source/Saved	  | Хранение обновляемых файлов                              |
| source/tecs	  | Хранение вспомогательных заголовочных файлов             |
| source/Utilities| Хранение утилит                                          |
