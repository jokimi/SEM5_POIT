class Program
{
    static double MySleep(int ms)
    {
        double sum = 0, temp;

        // Преобразование миллисекунд в такты
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

    static void Main(string[] args)
    {
        Console.WriteLine("Начинается выполнение MySleep(10000)...");
        double result = MySleep(10000); // 10 секунд
        Console.WriteLine("Выполнено. Результат: " + result);
    }
}