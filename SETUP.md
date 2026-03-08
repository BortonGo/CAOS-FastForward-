## Настройка окружения

Мы рекомендуем использовать Ubuntu версии **24.04**. Мы проверяем
все задачи на этой версии операционной системы, и если вы будете использовать эту же версию
локально, то вы избежите многих проблем с неправильной настройкой окружения. Мы компилируем всё с **gcc**.

### Windows 

Устанавливаете WSL2 через командую строку PowerShell `wsl --install`. [Официальная инструкция.](https://learn.microsoft.com/en-us/windows/wsl/install)

Из [Microsoft Store](https://apps.microsoft.com/detail/9pdxgncfsczv?ocid=webpdpshare) утсаналиваете Ubuntu 24.04 ли новее. Перезагружаете компьютер (желательно).

В меню пуск находите Ubuntu и запускаете - откроется linux консоль. Далее аналогично инструкции для Linux

### Linux

Вы можете на свое усмотрение использовать `venv`, однако это не требуется.

`apt update && apt install -y python3 python3-pip` (для не Debian/Ubuntu используйте друго пакетный менеджер)

`python3 -m pip install pyyml` (требуется только для Python 3.12 и ниже, на Ubuntu также требуется в конец флаг `--break-system-packages`)

Если хотите тестировать в docker ровно как на сервере - из корня `docker build -t caos .`

### MacOS

Вы можете на свое усмотрение использовать `venv`, однако это не требуется.

`brew install python`

`python3 -m pip install pyyml` (требуется только для Python 3.12 и ниже)

### MacOS (ARM архитектура)

На компьютерах Mac с процессорами серии M (M1, M2 и т.д.) могут возникнуть проблемы с компиляцией ассемблерного кода, так как архитектура процессора другая.

Одно из решений: использовать Docker.

Для этого надо:

1) Установить [Docker](https://www.docker.com/)
2) Сбилдить Docker-образ

`docker build --platform linux/amd64 -t caos .` (запускать из корневой директории проекта)

3) Для локального тестирования в директориях с задачами запускать

`make docker-test`

вместо 

`make test`
## Предупреждение

Система предназначена только для сдачи заданий. Любая замеченная попытка нарушить работу системы или обойти тесты (читинг) приведет к бану на курсе.

К читингу приравниваются умышленные и неумышленные действия, которые привели к получению данных приватных данных тестов, данных других студентов, данных преподавателей.

## Регистрация в системе

1. Создайте аккаунт на https://gitlab.qcourse.ru/ с помощью бота.
2. Добавьте ssh-ключ в аккаунт:
    * Сгенерируйте ssh ключ, если у вас его еще нет.
     ```bash
     ssh-keygen -N "" -f ~/.ssh/id_rsa
     ```
    * Скопируйте содержимое файла id_rsa.pub (`cat ~/.ssh/id_rsa.pub`) в https://gitlab.qcourse.ru/-/user_settings/ssh_keys
    * Проверьте, что ssh ключ работает. Выполните команду `ssh git@gitlab.qcourse.ru`. Вы должны увидеть такое приветствие:
     ```bash
     $ ssh git@gitlab.qcourse.ru
     PTY allocation request failed on channel 0
     Welcome to GitLab, $USERNAME!
     Connection to gitlab.qcourse.ru closed.
     ```

3. Зарегистрируйтесь на https://manytask.qcourse.ru/. Секрет для регистрации выдаст семинарист. Для авторизации на сайте используйте подготовленный выше аккаунт GitLab.

4. Для вас создастся приватный репозиторий, ссылка на который доступна из сайта https://manytask.qcourse.ru с дедлайнами (ссылка `My Repo`). Имя репозитория будет иметь вид, аналогичный `gitlab_login`. Проверьте, что вы имеете к нему доступ. В дальнейшем вам надо будет работать именно с вашим приватным репозиторием, а не с общим.

## Подготовка локального репозитория

1. Склонируйте ваш приватный репозиторий. Ссылку для клонирования можно найти, нажав на синюю кнопку `Clone -> Clone with SSH` в интерфейсе репозитория. Команда будет иметь вид
   ```bash
   git clone git@gitlab.qcourse.ru/caos-fastforward/students/{gitlab_login}.git
   ```

   Команда `git clone` создаст директорию вида `gitlab_login` и запишет туда все файлы из этого репозитория.

2. Настройте пользователя в git
   ```bash
   git config --global user.name "Vasya Pupkin"
   git config --global user.email vasya@pupkin.ru
   ```

3. Поставим себе `upstream` для обновлений
   ```bash
   git remote add upstream git@gitlab.qcourse.ru:caos-fastforward/public.git
   ```

4. Убедимся что все верно
   ```bash
   git remote -v
   ```
   
   Должен показать

   ```
   origin  git@gitlab.qcourse.ru:caos-fastforward/students/{gitlab_login}.git (fetch)
   origin  git@gitlab.qcourse.ru:caos-fastforward/students/{gitlab_login}.git (push)
   upstream  git@gitlab.qcourse.ru:caos-fastforward/public.git (fetch)
   upstream  git@gitlab.qcourse.ru:caos-fastforward/public.git (push)
   ```

## Сдача задач

Чтобы сдать задачу `task_name`, надо сделать следующие шаги:

1. Перейти на ветку `main`:
   ```bash
   git checkout main
   ```

2. Написать код решения задачи.

3. Проверить свое решение локальными тестами в директории задания. Делается командой:
   ```bash
   cd tasks/asm1/a_plus_b
   make test
   ```
   
   или через docker образ
   ```bash
   make docker-test
   ```

Если думаете, что решение верное и локальные тесты пройдены, то идем дальше!

4. Найти измененные файлы
   ```bash
   git status
   ```

5. Добавить измененные файлы задания, которое хотите отправить `file1`, `file2` и т.д. в коммит
   ```bash
   git add file1 file2
   ```

7. Закоммитить изменения. Старайтесь писать осмысленные сообщения к коммитам. Это поможет вам, когда вы захотите разобраться в истории своих изменений.
   ```bash
   git commit -m "Describe your changes here"
   ```

8. Запушить изменения в ваш приватный репозиторий
   ```bash
   git push
   ```
   
   или 

   ```bash
   git push origin main
   ```

## Синхронизация репозиториев

Периодически мы будем обновлять общий репозиторий с задачами, в ваш приватный репозиторий новые задачи попадут автоматически. Чтобы скачать обновления в ваш локальный репозиторий нужно выполнить команды:

   ```bash
   git fetch upstream
   git pull upstream main # получить обновления репозитория на ветку main
   ```