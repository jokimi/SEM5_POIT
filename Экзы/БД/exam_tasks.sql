SELECT CDB FROM v$database;
ALTER SESSION SET CONTAINER = ORCLPDB1;
ALTER SESSION SET CONTAINER = ORCLCDB;

-- СЛОЖНЫЕ

-- 1) Создайте процедуру, которая добавляет заказ. Обработайте возможные ошибки.
--    Создайте триггер, который контролирует целостность данных при добавлении заказа.

CREATE OR REPLACE PROCEDURE AddOrder(
    p_order_num IN KEM.ORDERS.ORDER_NUM%TYPE,
    p_order_date IN KEM.ORDERS.ORDER_DATE%TYPE,
    p_cust IN KEM.ORDERS.CUST%TYPE,
    p_rep IN KEM.ORDERS.REP%TYPE,
    p_mfr IN KEM.ORDERS.MFR%TYPE,
    p_product IN KEM.ORDERS.PRODUCT%TYPE,
    p_qty IN KEM.ORDERS.QTY%TYPE,
    p_amount IN KEM.ORDERS.AMOUNT%TYPE
) IS
    v_qty_on_hand KEM.PRODUCTS.QTY_ON_HAND%TYPE;
BEGIN
    SELECT QTY_ON_HAND INTO v_qty_on_hand FROM KEM.PRODUCTS WHERE MFR_ID = p_mfr AND PRODUCT_ID = p_product;
    IF v_qty_on_hand < p_qty THEN
        RAISE_APPLICATION_ERROR(-20001, 'Insufficient quantity on hand.');
    END IF;
    INSERT INTO KEM.ORDERS (ORDER_NUM, ORDER_DATE, CUST, REP, MFR, PRODUCT, QTY, AMOUNT)
        VALUES (p_order_num, p_order_date, p_cust, p_rep, p_mfr, p_product, p_qty, p_amount);
    UPDATE KEM.PRODUCTS SET QTY_ON_HAND = QTY_ON_HAND - p_qty WHERE MFR_ID = p_mfr AND PRODUCT_ID = p_product;
    COMMIT;
EXCEPTION
    WHEN OTHERS THEN
        ROLLBACK;
        RAISE_APPLICATION_ERROR(-20002, 'Error adding order: ' || SQLERRM);
END;

CREATE OR REPLACE TRIGGER CheckOrderIntegrity
    BEFORE INSERT ON KEM.ORDERS
    FOR EACH ROW
DECLARE
    v_cust_exists NUMBER;
    v_product_exists NUMBER;
BEGIN
    -- Проверяем, существует ли клиент
    SELECT COUNT(*) INTO v_cust_exists FROM KEM.CUSTOMERS WHERE CUST_NUM = :NEW.CUST;
    IF v_cust_exists = 0 THEN
        RAISE_APPLICATION_ERROR(-20003, 'Customer does not exist.');
    END IF;
    -- Проверяем, существует ли продукт
    SELECT COUNT(*) INTO v_product_exists FROM KEM.PRODUCTS WHERE MFR_ID = :NEW.MFR AND PRODUCT_ID = :NEW.PRODUCT;
    IF v_product_exists = 0 THEN
        RAISE_APPLICATION_ERROR(-20004, 'Product does not exist.');
    END IF;
END;

BEGIN
    ADDORDER(
        113072,                                  -- номер заказа
        TO_DATE('2025-01-24', 'YYYY-MM-DD'),    -- дата заказа
        2101,                                         -- номер клиента
        106,                                           -- номер продавца
        'ACI',                                         -- производитель
        '41003',                                   -- продукт
        5,                                             -- количество
        535.00                                      -- сумма
    );
END;

select* from KEM.orders;

-- 2) Создайте функцию, которая возвращает количество заказов покупателя помесячно за определенный период.
--    Параметры – покупатель, дата начала периода, дата окончания периода. Обработайте возможные ошибки.

CREATE OR REPLACE FUNCTION GetMonthlyOrdersCount(
    p_cust_num IN KEM.ORDERS.CUST%TYPE,
    p_start_date IN DATE,
    p_end_date IN DATE
) RETURN SYS_REFCURSOR IS
    v_result SYS_REFCURSOR;
BEGIN
    OPEN v_result FOR
        SELECT TO_CHAR(ORDER_DATE, 'YYYY-MM') AS order_month, COUNT(*) AS order_count
            FROM KEM.ORDERS WHERE CUST = p_cust_num AND ORDER_DATE BETWEEN p_start_date AND p_end_date
            GROUP BY TO_CHAR(ORDER_DATE, 'YYYY-MM')
            ORDER BY order_month;
    RETURN v_result;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20005, 'Error fetching monthly orders: ' || SQLERRM);
END;

DECLARE
    v_result SYS_REFCURSOR;
    v_order_month VARCHAR2(7);
    v_order_count NUMBER;
BEGIN
    v_result := GetMonthlyOrdersCount(2101, TO_DATE('2025-01-23', 'YYYY-MM-DD'), TO_DATE('2025-12-31', 'YYYY-MM-DD'));
    LOOP
        FETCH v_result INTO v_order_month, v_order_count; -- Получаем данные из курсора
        EXIT WHEN v_result%NOTFOUND;                     -- Завершаем цикл, если данные закончились
        DBMS_OUTPUT.PUT_LINE(v_order_month || ': ' || v_order_count); -- Вывод результата
    END LOOP;
    CLOSE v_result;
END;

-- 3) Создайте процедуру, которая выводит в консоль список всех товаров, не приобретенных ни одним покупателем в определенном году
--    по убыванию количества на складе. Параметр – год. Обработайте возможные ошибки.

CREATE OR REPLACE PROCEDURE GetUnsoldProductsByYear(
    p_year IN NUMBER
) IS
BEGIN
    FOR rec IN (
        SELECT P.MFR_ID, P.PRODUCT_ID, P.DESCRIPTION, P.QTY_ON_HAND FROM KEM.PRODUCTS P
            WHERE NOT EXISTS (
                SELECT 1 FROM KEM.ORDERS O
                    WHERE O.MFR = P.MFR_ID AND O.PRODUCT = P.PRODUCT_ID AND EXTRACT(YEAR FROM O.ORDER_DATE) = p_year
        )
        ORDER BY P.QTY_ON_HAND DESC
    ) LOOP
        DBMS_OUTPUT.PUT_LINE('MFR_ID: ' || rec.MFR_ID || ', PRODUCT_ID: ' || rec.PRODUCT_ID ||
            ', DESCRIPTION: ' || rec.DESCRIPTION || ', QTY_ON_HAND: ' || rec.QTY_ON_HAND);
    END LOOP;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20006, 'Error fetching unsold products: ' || SQLERRM);
END;

BEGIN
    GETUNSOLDPRODUCTSBYYEAR(P_YEAR => 2024);
END;

-- 4) Создайте функцию, которая подсчитывает количество заказов покупателя за определенный год.
--    Параметры – год, часть имени покупателя или код.

CREATE OR REPLACE FUNCTION GetCustomerOrdersCount(
    p_year IN NUMBER,
    p_customer_filter IN VARCHAR2
) RETURN NUMBER IS
    v_order_count NUMBER;
BEGIN
    SELECT COUNT(*) INTO v_order_count FROM KEM.ORDERS O
        JOIN KEM.CUSTOMERS C ON O.CUST = C.CUST_NUM
        WHERE EXTRACT(YEAR FROM O.ORDER_DATE) = p_year AND (C.COMPANY LIKE '%' || p_customer_filter || '%'
            OR C.CUST_NUM = TO_NUMBER(p_customer_filter));
    RETURN v_order_count;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20007, 'Error calculating customer orders: ' || SQLERRM);
END;

SELECT GetCustomerOrdersCount(2025, 2101) AS ORDERS_COUNT FROM DUAL;

-- 5) Создайте процедуру, которая сортирует таблицу по определенному столбцу.
--    Параметры – название столбца, порядок сортировки (ASC, DESC). Обработайте возможные ошибки.

CREATE OR REPLACE PROCEDURE SortTable(
    p_table_name IN VARCHAR2,
    p_column_name IN VARCHAR2,
    p_sort_order IN VARCHAR2,
    p_result OUT SYS_REFCURSOR
) IS
    v_sql VARCHAR2(4000);
BEGIN
    -- Формируем динамический SQL-запрос
    v_sql := 'SELECT * FROM ' || p_table_name || ' ORDER BY ' || p_column_name || ' ' || p_sort_order;
    -- Открываем курсор
    OPEN p_result FOR v_sql;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20008, 'Error sorting table: ' || SQLERRM);
END;

DECLARE
    v_result SYS_REFCURSOR;
    v_row KEM.OFFICES%ROWTYPE; -- Структура строки таблицы OFFICES
BEGIN
    SortTable('KEM.OFFICES', 'SALES', 'DESC', v_result);
    LOOP
        FETCH v_result INTO v_row;
        EXIT WHEN v_result%NOTFOUND;
        DBMS_OUTPUT.PUT_LINE('Office: ' || v_row.OFFICE || ', Sales: ' || v_row.SALES);
    END LOOP;
    CLOSE v_result;
END;

CREATE TABLE hard_5 ( c1 char(2), c2 char(2));
INSERT INTO hard_5 VALUES ('Fi', 'If');
INSERT INTO hard_5 VALUES ('Dd', 'dD');
INSERT INTO hard_5 VALUES ('Qi', 'Iq');
INSERT INTO hard_5 VALUES ('Aq', 'Qa');
INSERT INTO hard_5 VALUES ('Gf', 'Fg');
INSERT INTO hard_5 VALUES ('Aa', 'aA');
INSERT INTO hard_5 VALUES ('aa', 'UU');
INSERT INTO hard_5 VALUES ('Uu', 'AA');

CREATE OR REPLACE PROCEDURE procOrderTableHard5( columnName char, orderType varchar2)--COLUMN NAME => DYNAMIC CURSOR
is
    type c_type is ref cursor;
    c_mem c_type;
    o_memrow hard_5%rowtype;
    t_query varchar2(50) := 'SELECT * FROM hard_5 ORDER BY ';
begin
    if ((columnName != 'c1')AND(columnName != 'c2'))
        then RAISE_APPLICATION_ERROR(-20000, 'Incorrect column name');
    elsif ((orderType != 'ASC')AND(orderType != 'DESC'))
        then RAISE_APPLICATION_ERROR(-20001, 'Incorrect order type');
    end if;
    t_query := t_query||columnName||' '||orderType;
    open c_mem for t_query;
    DELETE hard_5;
    loop
        fetch c_mem into o_memrow;
        exit when c_mem%notfound;
        INSERT INTO hard_5 VALUES (o_memrow.c1, o_memrow.c2);
    end loop;
    commit;
exception
    when others
        then rollback;
            dbms_output.put_line('POTH5 error: '||sqlerrm);
end;
SELECT * FROM hard_5;
begin
    procOrderTableHard5('c2','ASC');
end;
SELECT * FROM hard_5;

-- 6) Процедура, которая выдаёт привилегию пользователю. Привилегия и пользователь передаются в качестве параметров

CREATE OR REPLACE PROCEDURE procGivePrivToUser(priv varchar2, usr varchar2)
is
    t_grant varchar2(300) := 'GRANT ';
begin
    t_grant := concat(t_grant, priv);
    t_grant := concat(t_grant, ' TO ');
    t_grant := concat(t_grant, usr);
    --t_grant := concat(t_grant, ';');
    execute immediate t_grant;
    commit;
    DBMS_OUTPUT.PUT_LINE(t_grant||' : success!');
exception
    when others
        then rollback;
            DBMS_OUTPUT.PUT_LINE(t_grant||' : ERROR: '||sqlerrm);
end;
create user JOJO_ref identified by oracle;
-- create connection - it wont work cause no priv!
begin
    procGivePrivToUser('CREATE SESSION','JOJO_ref');
end;
-- now connect as new user - it will be successful!
SELECT * FROM USER_SYS_PRIVS;
drop user JOJO_ref;

-- СРЕДНЕСЛОЖНЫЕ

-- 1) Создайте процедуру, которая выводит список заказов и их итоговую стоимость для определенного покупателя.
--    Параметр – наименование покупателя. Обработайте возможные ошибки.

CREATE OR REPLACE PROCEDURE GetOrdersForCustomer (
    customer_name IN VARCHAR2
)
IS
    CURSOR order_cursor IS
        SELECT o.ORDER_NUM, o.AMOUNT FROM KEM.ORDERS o JOIN KEM.CUSTOMERS c ON o.CUST = c.CUST_NUM
            WHERE c.COMPANY = customer_name;
    v_order_num KEM.ORDERS.ORDER_NUM%TYPE;
    v_amount KEM.ORDERS.AMOUNT%TYPE;
    v_total_amount NUMBER := 0;  -- Переменная для хранения общей суммы
BEGIN
    OPEN order_cursor;
    LOOP
        FETCH order_cursor INTO v_order_num, v_amount;
        EXIT WHEN order_cursor%NOTFOUND;    
        DBMS_OUTPUT.PUT_LINE('Order Number: ' || v_order_num || ', Amount: ' || v_amount);
        v_total_amount := v_total_amount + v_amount;  -- Суммируем стоимость заказов
    END LOOP;
    CLOSE order_cursor;
    DBMS_OUTPUT.PUT_LINE('Total Amount for ' || customer_name || ': ' || v_total_amount);
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Error: ' || SQLERRM);
END;

BEGIN
    GetOrdersForCustomer('JCP Inc.');
END;

-- 2) Создайте функцию, которая подсчитывает количество заказов покупателя за определенный период.
--    Параметры – покупатель, дата начала периода, дата окончания периода

CREATE OR REPLACE FUNCTION GetOrderCountForPeriod(
    customer_name IN VARCHAR2,
    start_date    IN DATE,
    end_date      IN DATE
) RETURN NUMBER
IS
    order_count NUMBER;
BEGIN
    SELECT COUNT(*) INTO order_count FROM KEM.ORDERS o JOIN KEM.CUSTOMERS c ON o.CUST = c.CUST_NUM
        WHERE c.COMPANY = customer_name AND o.ORDER_DATE BETWEEN start_date AND end_date;
    RETURN order_count;
EXCEPTION
    WHEN OTHERS THEN
        RETURN -1;
END;

SELECT GetOrderCountForPeriod('JCP Inc.', DATE '2007-01-01', DATE '2008-01-01') AS Amount_Of_Orders FROM DUAL;

-- 3) Создайте процедуру, которая выводит список всех товаров, приобретенных покупателем, с указанием суммы продаж по возрастанию.
--    Параметр – наименование покупателя. Обработайте возможные ошибки.

CREATE OR REPLACE PROCEDURE GetProductsForCustomer (
    customer_name IN VARCHAR2
)
IS
    CURSOR product_cursor IS
        SELECT p.DESCRIPTION, SUM(o.AMOUNT) AS total_amount FROM KEM.ORDERS o
            JOIN KEM.CUSTOMERS c ON o.CUST = c.CUST_NUM
            JOIN KEM.PRODUCTS p ON o.MFR = p.MFR_ID AND o.PRODUCT = p.PRODUCT_ID
            WHERE c.COMPANY = customer_name
            GROUP BY p.DESCRIPTION
            ORDER BY total_amount ASC;
    v_description KEM.PRODUCTS.DESCRIPTION%TYPE;
    v_total NUMBER;
BEGIN
    OPEN product_cursor;
    LOOP
        FETCH product_cursor INTO v_description, v_total;
        EXIT WHEN product_cursor%NOTFOUND;
        DBMS_OUTPUT.PUT_LINE('Product: ' || v_description || ', Total Sales: ' || v_total);
    END LOOP;
    CLOSE product_cursor;
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Error: ' || SQLERRM);
END;

BEGIN
    GetProductsForCustomer('JCP Inc.');
END;

-- 4) Создайте функцию, которая добавляет покупателя в таблицу Customers, и возвращает код добавленного покупателя или -1 в случае ошибки.
--    Параметры соответствуют столбцам таблицы, кроме кода покупателя, который задается при помощи последовательности.

CREATE SEQUENCE KEM.CUSTOMERS_SEQ
    START WITH 1
    INCREMENT BY 1
    NOCACHE
    NOCYCLE;
drop sequence KEM.CUSTOMERS_SEQ;

CREATE OR REPLACE FUNCTION AddCustomer (
    company_name   IN VARCHAR2,
    cust_rep       IN NUMBER,
    credit_limit   IN NUMBER
) RETURN NUMBER
IS
    new_customer_id NUMBER;
BEGIN
    SELECT KEM.CUSTOMERS_SEQ.NEXTVAL INTO new_customer_id FROM DUAL;
    INSERT INTO KEM.CUSTOMERS (CUST_NUM, COMPANY, CUST_REP, CREDIT_LIMIT)
        VALUES (new_customer_id, company_name, cust_rep, credit_limit);
    RETURN new_customer_id;
EXCEPTION
    WHEN OTHERS THEN
        RETURN -1;
END;

DECLARE
    newcode NUMBER;
BEGIN
    newcode := AddCustomer('New Customer', 101, 50000.00);
    DBMS_OUTPUT.PUT_LINE('New customer code: ' || newcode);
END;

-- 5) Создайте процедуру, которая выводит список покупателей, в порядке убывания общей стоимости заказов.
--    Параметры – дата начала периода, дата окончания периода. Обработайте возможные ошибки.

CREATE OR REPLACE PROCEDURE GetCustomersByOrderValue (
    start_date IN DATE,
    end_date   IN DATE
)
IS
    CURSOR customer_cursor IS
        SELECT c.COMPANY, SUM(o.AMOUNT) AS total_sales FROM KEM.ORDERS o
            JOIN KEM.CUSTOMERS c ON o.CUST = c.CUST_NUM
            WHERE o.ORDER_DATE BETWEEN start_date AND end_date
            GROUP BY c.COMPANY
            ORDER BY total_sales DESC;
    v_company KEM.CUSTOMERS.COMPANY%TYPE;
    v_total_sales NUMBER;
BEGIN
    OPEN customer_cursor;
    LOOP
        FETCH customer_cursor INTO v_company, v_total_sales;
        EXIT WHEN customer_cursor%NOTFOUND;
        DBMS_OUTPUT.PUT_LINE('Customer: ' || v_company || ', Total Sales: ' || v_total_sales);
    END LOOP;
    CLOSE customer_cursor;
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Error: ' || SQLERRM);
END;

BEGIN
    GetCustomersByOrderValue(DATE '2007-01-01', DATE '2008-12-31'); -- Укажите нужный период.
END;

-- 6) Создайте функцию, которая подсчитывает количество заказанных товаров за определенный период.
--    Параметры – дата начала периода, дата окончания периода.

CREATE OR REPLACE FUNCTION GetProductOrderCount (
    start_date IN DATE,
    end_date IN DATE
) RETURN NUMBER
IS
    total_qty NUMBER;
BEGIN
    SELECT SUM(QTY) INTO total_qty FROM KEM.ORDERS
        WHERE ORDER_DATE BETWEEN start_date AND end_date;
    RETURN total_qty;
EXCEPTION
    WHEN OTHERS THEN
        RETURN -1;
END;

SELECT GetProductOrderCount(DATE '2007-01-01', DATE '2008-12-31') AS Amount_Of_Products FROM DUAL;

-- 7) Создайте процедуру, которая выводит список покупателей, у которых есть заказы в этом временном периоде.
--    Параметры – дата начала периода, дата окончания периода. Обработайте возможные ошибки

CREATE OR REPLACE PROCEDURE GetCustomersWithOrders (
    start_date IN DATE,
    end_date   IN DATE
)
IS
    CURSOR customer_cursor IS
        SELECT DISTINCT c.COMPANY FROM KEM.ORDERS o JOIN KEM.CUSTOMERS c ON o.CUST = c.CUST_NUM
            WHERE o.ORDER_DATE BETWEEN start_date AND end_date;
    v_company KEM.CUSTOMERS.COMPANY%TYPE;
BEGIN
    OPEN customer_cursor;
    LOOP
        FETCH customer_cursor INTO v_company;
        EXIT WHEN customer_cursor%NOTFOUND;
        DBMS_OUTPUT.PUT_LINE('Customer: ' || v_company);
    END LOOP;
    CLOSE customer_cursor;
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Error: ' || SQLERRM);
END GetCustomersWithOrders;

BEGIN
    GetCustomersWithOrders(DATE '2007-01-01', DATE '2008-12-31'); -- Укажите нужный период.
END;

-- 8) Создайте функцию, которая подсчитывает количество покупателей определенного товара. Параметры – наименование товара.

CREATE OR REPLACE FUNCTION GetCustomerCountForProduct (
    product_name IN VARCHAR2
) RETURN NUMBER
IS
    customer_count NUMBER;
BEGIN
    SELECT COUNT(DISTINCT o.CUST) INTO customer_count FROM KEM.ORDERS o
        JOIN KEM.PRODUCTS p ON o.MFR = p.MFR_ID AND o.PRODUCT = p.PRODUCT_ID
        WHERE p.DESCRIPTION = product_name;
    RETURN customer_count;
EXCEPTION
    WHEN OTHERS THEN
        RETURN -1;
END;

SELECT GetCustomerCountForProduct('Widget Remover') AS Amount_Of_Customers FROM DUAL;

-- 9) Создайте процедуру, которая увеличивает на 10% стоимость определенного товара. Параметр – наименование товара. Обработайте возможные ошибки

CREATE OR REPLACE PROCEDURE IncreaseProductPrice (
    product_name IN VARCHAR2
)
IS
    v_new_price KEM.PRODUCTS.PRICE%TYPE;  -- Переменная для хранения новой цены
BEGIN
    UPDATE KEM.PRODUCTS SET PRICE = PRICE * 1.10 WHERE DESCRIPTION = product_name 
        RETURNING PRICE INTO v_new_price;
    IF SQL%ROWCOUNT = 0 THEN
        DBMS_OUTPUT.PUT_LINE('Product not found: ' || product_name);
    ELSE
        DBMS_OUTPUT.PUT_LINE('Price updated for product: ' || product_name || ', New Price: ' || v_new_price);
    END IF;
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Error: ' || SQLERRM);
END;

BEGIN
    IncreaseProductPrice('Widget Remover');
END;

-- 10) Создайте функцию, которая вычисляет количество заказов, выполненных в определенном году для определенного покупателя.
--     Параметры – покупатель, год. товара.

CREATE OR REPLACE FUNCTION GetOrderCountForYear (
    customer_name IN VARCHAR2,
    year_value IN NUMBER
) RETURN NUMBER
IS
    order_count NUMBER;
BEGIN
    SELECT COUNT(*) INTO order_count FROM KEM.ORDERS o JOIN KEM.CUSTOMERS c ON o.CUST = c.CUST_NUM
        WHERE c.COMPANY = customer_name
            AND EXTRACT(YEAR FROM o.ORDER_DATE) = year_value;
    RETURN order_count;
EXCEPTION
    WHEN OTHERS THEN
        RETURN -1;
END;

SELECT GetOrderCountForYear('JCP Inc.', 2007) AS Amount_Of_Orders FROM DUAL;

-- ЛЁГКИЕ

-- 1. Определите размеры областей памяти SGA. 

SELECT * FROM V$SGA;

-- 2. Получите список всех параметров экземпляра. 

SELECT * FROM V$PARAMETER;

-- 3. Получите список управляющих файлов. 

SHOW PARAMETER control_files;

-- 4. Сформируйте PFILE.

CREATE PFILE = 'opt/oracle/product/21c/dbhome_1/dbs/initfile.ora' FROM SPFILE;

-- 5. Создайте таблицу из двух столбцов, один из которых первичный ключ.
--    Получите перечень всех сегментов. Вставьте данные в таблицу.
--    Определите, сколько в сегменте таблицы экстентов, их размер в блоках и байтах.

CREATE TABLE my_table (
    id NUMBER PRIMARY KEY,
    name VARCHAR2(50)
);
drop my_table;
INSERT INTO my_table (id, name) VALUES (1, 'First');
INSERT INTO my_table (id, name) VALUES (2, 'Second');
COMMIT;
SELECT * FROM USER_SEGMENTS WHERE SEGMENT_NAME = 'MY_TABLE';
SELECT EXTENT_ID, BYTES, BLOCKS FROM USER_EXTENTS WHERE SEGMENT_NAME = 'MY_TABLE';

-- 6. Получите перечень всех процессов СУБД Oracle. Для серверных процессов укажите режим подключения.
--    Для фоновых укажите работающие в настоящий момент.

SELECT * FROM V$PROCESS;
SELECT SID, SERIAL#, USERNAME, STATUS FROM V$SESSION;

-- 7. Получите перечень всех табличных пространств и их файлов. 

SELECT TABLESPACE_NAME, FILE_NAME, BYTES/1024/1024 AS SIZE_MB FROM DBA_DATA_FILES;

-- 8. Получите перечень всех ролей.

SELECT * FROM DBA_ROLES;

-- 9. Получите перечень привилегий для определенной роли.

SELECT * FROM ROLE_SYS_PRIVS WHERE ROLE = 'DBA';

-- 10. Получите перечень всех пользователей.

SELECT USERNAME FROM DBA_USERS;

-- 11. Создайте роль.

CREATE ROLE new_role;

-- 12. Создайте пользователя.

CREATE USER jokies IDENTIFIED BY jokies;
GRANT CONNECT TO jokies;

-- 13. Получите перечень всех профилей безопасности.

SELECT * FROM DBA_PROFILES;

-- 14. Получите перечень всех параметров профиля безопасности.

SELECT * FROM DBA_PROFILES WHERE PROFILE = 'DEFAULT';

-- 15. Создайте профиль безопасности.

CREATE PROFILE my_profile LIMIT 
    FAILED_LOGIN_ATTEMPTS 5
    PASSWORD_LIFE_TIME 30;

-- 16. Создайте последовательность S1, со следующими характеристиками:
--     начальное значение 1000; приращение 10; минимальное значение 0; максимальное значение 10000;
--     циклическую; кэширующую 30 значений в памяти; гарантирующую хронологию значений.
--     Создайте таблицу T1 с тремя столбцами и введите (INSERT) 10 строк, со значениями из S1.

CREATE SEQUENCE S1
    START WITH 1000
    INCREMENT BY 10
    MINVALUE 0
    MAXVALUE 10000
    CYCLE
    CACHE 30;
CREATE TABLE T1 (
    col1 NUMBER,
    col2 NUMBER,
    col3 NUMBER
);
BEGIN
    FOR i IN 1..10 LOOP
        INSERT INTO T1 (col1, col2, col3) VALUES (S1.NEXTVAL, S1.NEXTVAL, S1.NEXTVAL);
    END LOOP;
END;

-- 17. Создайте частный и публичный синоним для одной из таблиц и продемонстрируйте его область видимости.
--     Найдите созданные синонимы в представлениях словаря Oracle.

CREATE PUBLIC SYNONYM my_table_syn FOR my_table;
CREATE SYNONYM my_private_syn FOR my_table;
SELECT * FROM DBA_SYNONYMS WHERE OWNER = 'PUBLIC';
SELECT * FROM USER_SYNONYMS;
SELECT * FROM DBA_SYNONYMS WHERE TABLE_NAME = 'MY_TABLE';

-- 18. Разработайте анонимный блок, демонстрирующий возникновение и обработку исключений WHEN TO_MANY_ROWS и NO_DATA_FOUND.

select * from my_table;
insert into my_table values (1, 'hi');
insert into my_table values (2, 'hi');

DECLARE
    v_value my_table.name%TYPE;  -- Используем тип столбца для v_value
BEGIN
    SELECT name INTO v_value FROM my_table WHERE name = 'hi';  -- Извлекаем конкретное значение
    DBMS_OUTPUT.PUT_LINE('Value: ' || v_value);
EXCEPTION
    WHEN NO_DATA_FOUND THEN
        DBMS_OUTPUT.PUT_LINE('No data found.');
    WHEN TOO_MANY_ROWS THEN
        DBMS_OUTPUT.PUT_LINE('Too many rows found.');
END;

-- 19. Получите перечень всех файлов групп журналов повтора. 

SELECT * FROM V$LOGFILE;

-- 20. Определите текущую группу журналов повтора.

SELECT GROUP#, STATUS FROM V$LOG;

-- 21. Получите перечень контрольных файлов.

SELECT * FROM V$CONTROLFILE;

-- 22. Создайте таблицу и вставьте в нее 100 записей. Найдите таблицу и ее свойства в представлениях словаря.

CREATE TABLE table_100_records (id NUMBER, data VARCHAR2(50));
BEGIN
    FOR i IN 1..100 LOOP
        INSERT INTO table_100_records VALUES (i, 'Data ' || i);
    END LOOP;
    COMMIT;
END;
SELECT * FROM USER_TABLES WHERE TABLE_NAME = 'TABLE_100_RECORDS';

-- 23. Получите список сегментов табличного пространства.

SELECT SEGMENT_NAME, SEGMENT_TYPE FROM DBA_SEGMENTS WHERE TABLESPACE_NAME = 'SYSTEM';

-- 24. Выведите список всех объектов, доступных пользователю. 

SELECT OBJECT_NAME, OBJECT_TYPE FROM USER_OBJECTS;

-- 25. Вычислите количество блоков, занятых таблицей.

SELECT BLOCKS FROM DBA_SEGMENTS WHERE SEGMENT_NAME = 'TABLE_100_RECORDS';

-- 26. Выведите список текущих сессий. 

SELECT SID, USERNAME, STATUS FROM V$SESSION;

-- 27. Выведите, производится ли архивирование журналов повтора.

SELECT LOG_MODE FROM V$DATABASE;

-- 28. Создайте представление с определенными параметрами.

CREATE VIEW my_view AS SELECT * FROM my_table WHERE id > 2;

-- 29. Создайте database link с определенными параметрами.

CREATE DATABASE LINK remote_db CONNECT TO remote_user IDENTIFIED BY password USING 'remote_service';
drop DATABASE LINK remote_db;

-- 30. Продемонстрируйте эскалацию исключения.

BEGIN
    BEGIN
        RAISE NO_DATA_FOUND;
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            RAISE; -- Эскалируем исключение.
    END;
EXCEPTION
    WHEN NO_DATA_FOUND THEN
        DBMS_OUTPUT.PUT_LINE('Exception escalated.');
END;
