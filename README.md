# ProducerConsumer1
Есть поток Producer, который читает файл построчно, каждую строку  в некий буфер(sourceQueue), и есть потоки Сonsumers, 
которые извлекают строки из буфера и суммируют их к общей переменной totalSum, таким образом, получаем сумму чисел в файле(source.txt).
Компиляция:
gcc  main.c queueChar.c  -o main -lpthread
