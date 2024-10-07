class Program
{
    static int Count = 0;
    static readonly object lockObject = new object(); // Для синхронизации

    static void WorkThread()
    {
        for (int i = 0; i < 5000000; ++i)
        {
            lock (lockObject) // Блокировка для обеспечения безопасности
            {
                Count++;
            }
        }
    }

    static void Main(string[] args)
    {
        Thread[] threads = new Thread[20];

        // Запуск 20 потоков
        for (int i = 0; i < 20; ++i)
        {
            threads[i] = new Thread(WorkThread);
            threads[i].Start();
        }

        // Ожидание завершения всех потоков
        for (int i = 0; i < 20; ++i)
        {
            threads[i].Join();
        }

        Console.WriteLine("Результат Count: " + Count);
        Console.WriteLine("Ожидаемое значение: " + (20 * 5000000));
    }
}