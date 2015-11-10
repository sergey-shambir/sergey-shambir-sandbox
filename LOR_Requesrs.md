## An opinion about what really good Python IDE should provide ##
  * Браузер классов (ну это само-собой)
  * Встроенный REPL в котором можно запустить код выделенный в редакторе
  * Возможность запуска проектов на отладку
  * Понимание такой вещи как she-bang (или как оно там правильно зовется? из головы вылетело), чтоб можно было открывать файлы без расширения `*.py` (ни в pydev ни в netbeans такого не нашел)
  * Встроенный браузер документации, желательно, чтоб он умел показывать не только html/txt, но и pdf/djvu/chm и чтоб можно было открыть любой произвольный документ с диска
  * Автодополнение — по возможности

## Question on linux.org.ru/forum/development ##
В каких питоновских ide есть:
  1. загрузка файла в repl. А также выделенной строки, блока кода, выделенного региона.
  1. автодоплнение с подсказками и документацией
  1. удобное переключение между буферами (например, по имени)
  1. графическая полоска выполнения тестов

## Another imilar question ##
Посоветуйте сабж со следующими возможностями:

Главное: автодополнение, чтобы оно брало слова не только из текущего файла, а из стандартной библиотеки python и изо всех импортированных в редактируемом файле модулей.

Не обязательно, но желательо: интеграция с PyQt, чтобы примерно как в QtCreator. Главное — чтобы облегчало добавление в проект форм Qt Designer, а то вручную много писать приходится.

## Question about python IDE with good autocompletion ##
Посоветуйте IDE для Python'а с нормальным автодополнением. (Другие языки не предлагать!)

Пробовал:
- Netbeans: В качестве вариантов выводит все подряд, что совпадает по первым буквам в названии, даже из неимпортированных модулей. (Например, пишу на gtk, постоянно предлагает варианты из wxWidgets)
- SPE: В качестве вариантов предлагает только то, что уже было написано или то, что находится непосредственно в модуле.
import gtk

b = gtk.Button() <- Тут автодополнение работает
b.show() <- А тут уже не работает.

- Eclipse + pydev
Автодополнение работает не для всех модуле и почему-то медленно, иногда подвешивается на 5-15 секунд.

## Another question about IDE ##
Кто и что использует в роли сабжа? Желательно возможность создания проекта, автокомплит из собственных классов, просмоторщики методов и свойств класса.

## Comment to news about QtCreator 2.3 final release ##
Прочитал топик на lorcode.

В итоге пришли к тому, что делать плагин для Python/PyQt/PySide вроде как и не нужно? :)

Вообще-то было бы здорово, если бы QtCreator работал с Python и PyQt/PySide. Вот только сделать качественную реализацию наверное довольно сложно. Вот что сразу приходит на ум из нужных фич (пишу тут, потому что на lorcode невозможно зарегистрироваться):

  * Менеджер проектов с Source Browser и прочим
  * Автоматическое форматирование кода и умный indent
  * Умное автодополнение (как недавно запилили в плагине для VisualStudio)
  * Работающее автодополнение и вытаскивание доков из pyd-модулей
  * Интегрирование интерактивной консоли IPython со всеми её плюшками
  * Рефакторинг
  * Отладчик с визуальным Call Stack и возможностью интерактивной пробы (как в WingIDE)
  * Профайлер
  * Интерактивная справочная система (как Source Assistant в WingIDE)
  * Инструмент для создания standalone сборок (типа PyInstaller)
  * Остальные вкусности, которые есть почти в любой современной IDE