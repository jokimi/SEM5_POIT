class Program
{
    const int TaskCount = 8;         // Количество задач (для 8 логических процессоров)
    const int TaskLifeTime = 10;     // Время жизни каждой задачи в секундах
    const int ObservationTime = 30;   // Время наблюдения в секундах
    static int[,] Matrix = new int[TaskCount, ObservationTime]; // Матрица для хранения данных
    static DateTime StartTime = DateTime.Now;

    // Метод для выполнения длительных вычислений
    static double MySleep(int ms)
    {
        double sum = 0, temp;
        int endTicks = Environment.TickCount + ms;

        while (Environment.TickCount < endTicks)
        {
            for (int t = 0; t < 100; t++)
            {
                temp = 0.711 + (double)t / 10000.0;
                double a, b, c, d, e, nt;

                for (int k = 0; k < 5500; k++)
                {
                    nt = temp - k / 27000.0;
                    a = Math.Sin(nt);
                    b = Math.Cos(nt);
                    c = Math.Cos(nt / 2.0);
                    d = Math.Sin(nt / 2);
                    e = Math.Abs(1.0 - a * a - b * b) + Math.Abs(1.0 - c * c - d * d);
                    sum += e; // Нагружение процессора
                }
            }
        }

        return sum;
    }

    static void Work(int id)
    {
        DateTime threadStartTime = DateTime.Now;

        while ((DateTime.Now - threadStartTime).TotalSeconds < TaskLifeTime)
        {
            int elapsedSeconds = (int)(DateTime.Now - StartTime).TotalSeconds;
            if (elapsedSeconds < ObservationTime)
            {
                Matrix[id, elapsedSeconds] += 1; // Увеличим значение в матрице
            }
            MySleep(50); // Долгие вычисления
        }
    }

    static void Main(string[] args)
    {
        Task[] tasks = new Task[TaskCount];

        Console.WriteLine("A student ... is creating tasks...");

        // Создание и запуск задач по очереди
        for (int i = 0; i < TaskCount; i++)
        {
            int taskId = i; // Локальная переменная для замыкания
            tasks[i] = (i == 0)
                ? Task.Run(() => Work(taskId)) // Первая задача
                : tasks[i - 1].ContinueWith(t => Work(taskId)); // Остальные задачи
        }

        Console.WriteLine("A student ... is waiting for tasks to finish...");

        // Ожидание завершения всех задач
        Task.WaitAll(tasks);

        // Вывод результатов в виде таблицы
        Console.WriteLine("Время (с) | " + string.Join(" | ", Enumerable.Range(0, TaskCount).Select(i => $"Задача {i}")));
        Console.WriteLine(new string('-', 50));

        for (int s = 0; s < ObservationTime; s++)
        {
            Console.Write($"{s,3}:       | ");
            for (int th = 0; th < TaskCount; th++)
            {
                Console.Write($"{Matrix[th, s],5} | ");
            }
            Console.WriteLine();
        }
    }
}