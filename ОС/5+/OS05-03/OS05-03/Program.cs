using System;
using System.Diagnostics;
using System.Linq;
using System.Threading;

class Program
{
    const int ThreadCount = 16;
    const int ThreadLifeTime = 10;
    const int ObservationTime = 30;
    static int[,] Matrix = new int[ThreadCount, ObservationTime];
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

    static void WorkThread(object o)
    {
        int id = (int)o;
        DateTime threadStartTime = DateTime.Now;

        while ((DateTime.Now - threadStartTime).TotalSeconds < ThreadLifeTime)
        {
            int elapsedSeconds = (int)(DateTime.Now - StartTime).TotalSeconds;
            if (elapsedSeconds < ObservationTime)
            {
                Matrix[id, elapsedSeconds] += 1;
            }
            MySleep(50);
        }
    }

    //static void Main(string[] args)
    //{
    //    // Ограничение на количество используемых логических процессоров
    //    Process.GetCurrentProcess().ProcessorAffinity = (IntPtr)15;

    //    Thread[] threads = new Thread[ThreadCount];

    //    // Запуск потоков с установленными приоритетами
    //    for (int i = 0; i < ThreadCount; ++i)
    //    {
    //        threads[i] = new Thread(WorkThread);
    //        if (i < 4) // Первые 4 потока с минимальным приоритетом
    //        {
    //            threads[i].Priority = ThreadPriority.Lowest; // Минимальный приоритет
    //        }
    //        else
    //        {
    //            threads[i].Priority = ThreadPriority.Highest; // Максимальный приоритет
    //        }
    //        threads[i].Start(i); // Передаем идентификатор потока
    //    }

    //    Console.WriteLine("Главный поток ожидает завершения потоков...");

    //    // Ожидание завершения всех потоков
    //    for (int i = 0; i < ThreadCount; ++i)
    //    {
    //        threads[i].Join();
    //    }

    //    // Вывод результатов в виде таблицы
    //    Console.WriteLine("Время (с) | " + string.Join(" | ", Enumerable.Range(0, ThreadCount).Select(i => $"Поток {i}")));
    //    Console.WriteLine(new string('-', 50));

    //    for (int s = 0; s < ObservationTime; s++)
    //    {
    //        Console.Write($"{s,3}:       | ");
    //        for (int th = 0; th < ThreadCount; th++)
    //        {
    //            Console.Write($"{Matrix[th, s],5} | ");
    //        }
    //        Console.WriteLine();
    //    }
    //}

    static void Main(string[] args)
    {
        // Ограничение на количество используемых логических процессоров
        Process.GetCurrentProcess().ProcessorAffinity = (IntPtr)15;

        Thread[] threads = new Thread[ThreadCount];

        // Запуск потоков с установленными приоритетами
        for (int i = 0; i < ThreadCount; ++i)
        {
            threads[i] = new Thread(WorkThread);
            if (i > 12) // Первые два потока с приоритетом BelowNormal
            {
                threads[i].Priority = ThreadPriority.BelowNormal; // Ниже нормального
            }
            else
            {
                threads[i].Priority = ThreadPriority.Normal; // Нормальный приоритет
            }
            threads[i].Start(i); // Передаем идентификатор потока
        }

        Console.WriteLine("Главный поток ожидает завершения потоков...");

        // Ожидание завершения всех потоков
        for (int i = 0; i < ThreadCount; ++i)
        {
            threads[i].Join();
        }

        // Вывод результатов в виде таблицы
        Console.WriteLine("Время (с) | " + string.Join(" | ", Enumerable.Range(0, ThreadCount).Select(i => $"Поток {i}")));
        Console.WriteLine(new string('-', 50));

        for (int s = 0; s < ObservationTime; s++)
        {
            Console.Write($"{s,3}:       | ");
            for (int th = 0; th < ThreadCount; th++)
            {
                Console.Write($"{Matrix[th, s],5} | ");
            }
            Console.WriteLine();
        }
    }
}