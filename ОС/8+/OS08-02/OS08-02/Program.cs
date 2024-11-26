using System;
using System.Diagnostics;
using System.Threading;

class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("OS08_02: Создание объектов по 128 МБ каждые 5 секунд.");
        Console.WriteLine("Platform Target: x86\n");

        // Список для хранения ссылок на созданные объекты, чтобы предотвратить их сборку сборщиком мусора
        var objectList = new System.Collections.Generic.List<byte[]>();

        Thread memoryFillerThread = new Thread(() =>
        {
            Random random = new Random();
            while (true)
            {
                foreach (var obj in objectList)
                {
                    for (int i = 0; i < obj.Length; i++)
                    {
                        obj[i] = (byte)random.Next(256);
                    }
                }
                Thread.Sleep(1000);
            }
        });
        memoryFillerThread.IsBackground = true;
        memoryFillerThread.Start();

        int counter = 0;
        while (true)
        {
            counter++;

            byte[] bigObject = new byte[128 * 1024 * 1024];
            objectList.Add(bigObject);

            long memoryUsage = Process.GetCurrentProcess().PrivateMemorySize64;
            long workingSet = Process.GetCurrentProcess().WorkingSet64;

            Console.WriteLine($"[{counter}] Использование памяти: {memoryUsage / (1024 * 1024)} MB, Рабочий набор: {workingSet / (1024 * 1024)} MB");

            Thread.Sleep(5000);
        }
    }
}