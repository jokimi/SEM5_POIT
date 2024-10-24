from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

# Настройка опций для браузера
yandex_options = Options()
yandex_options.add_argument("--headless")  # Запуск в фоновом режиме (без графического интерфейса)

driver_path = "D:\\Applications\\environments\\yandexdriver.exe"
service = Service(driver_path)
driver = webdriver.Chrome(service=service, options=yandex_options)
driver.get("https://habr.com/ru/companies/otus/articles/596071/")

# Ожидание загрузки страницы
wait = WebDriverWait(driver, 10)

# Поиск элементов и вывод сообщений

# 1. По CSS-селекторам
try:
    element1 = wait.until(EC.visibility_of_element_located((By.CSS_SELECTOR, "h1.tm-title_h1")))
    print("Элемент найден (CSS):", element1.text)
except Exception as e:
    print("Ошибка при поиске элемента по CSS:", e)

try:
    element2 = wait.until(EC.visibility_of_element_located((By.CSS_SELECTOR, "div.tm-publication-label_variant-translation")))
    print("Элемент найден (CSS):", element2.text[:100])  # Выводим первые 100 символов
except Exception as e:
    print("Ошибка при поиске элемента по CSS:", e)

try:
    element3 = wait.until(EC.visibility_of_element_located((By.CSS_SELECTOR, "button.tm-footer__link")))
    print("Элемент найден (CSS):", element3.text)
except Exception as e:
    print("Ошибка при поиске элемента по CSS:", e)

# 2. По XPath
try:
    element1_xpath = wait.until(EC.visibility_of_element_located((By.XPATH, "//h1[contains(@class, 'tm-title_h1')]")))
    print("Элемент найден (XPath):", element1_xpath.text)
except Exception as e:
    print("Ошибка при поиске элемента по XPath:", e)

try:
    element2_xpath = wait.until(EC.visibility_of_element_located((By.XPATH, "//a[@class='tm-user-info__username']")))
    print("Элемент найден (XPath):", element2_xpath.text[:100])  # Выводим первые 100 символов
except Exception as e:
    print("Ошибка при поиске элемента по XPath:", e)

try:
    element3_xpath = wait.until(EC.visibility_of_element_located((By.XPATH, "//button[@class='tm-footer__link']")))
    print("Элемент найден (XPath):", element3_xpath.text)
except Exception as e:
    print("Ошибка при поиске элемента по XPath:", e)

# 3. По тегу
try:
    element_by_tag = wait.until(EC.visibility_of_element_located((By.TAG_NAME, "h1")))
    print("Элемент найден (по тегу):", element_by_tag.text)
except Exception as e:
    print("Ошибка при поиске элемента по тегу:", e)

# 4. По частичному тексту ссылки
try:
    element_partial_link = wait.until(EC.visibility_of_element_located((By.PARTIAL_LINK_TEXT, "selenium")))
    print("Элемент найден (по частичному тексту ссылки):", element_partial_link.text)
except Exception as e:
    print("Ошибка при поиске элемента по частичному тексту ссылки:", e)
    links = driver.find_elements(By.TAG_NAME, "a")
    print("Доступные ссылки на странице:")
    for link in links:
        print(link.text)

# Закрытие драйвера
driver.quit()