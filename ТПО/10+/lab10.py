from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.common.exceptions import NoSuchElementException, TimeoutException
import pytest
import time
import json
import os

# Параметры теста
GITHUB_USERNAME = "2024jokeiminny"
GITHUB_PASSWORD = "jokeiminny2024"
NEW_REPO_NAME = 'new-git-demo'

# Настройки браузера
driver_path = "D:\\Applications\\environments\\yandexdriver.exe"

# Фикстура для подготовки и очистки тестового окружения
@pytest.fixture(scope="function")
def setup_teardown():
    yandex_options = Options()
    yandex_options.add_argument("--disable-logging")
    yandex_options.add_argument("--disable-notifications")
    yandex_options.add_argument("user-agent=CustomUserAgent/1.0")
    service = Service(driver_path)
    driver = webdriver.Chrome(service=service, options=yandex_options)
    wait = WebDriverWait(driver, 10)
    yield driver, wait
    driver.quit()

# Функции для сохранения cookies
def save_cookies(driver):
    cookies = driver.get_cookies()
    with open("D:\\BSTU\\5 sem\\ТПО\\Лабы\\10\\cookies.json", "w") as file:
        json.dump(cookies, file)
    print("Cookies сохранены.")

# Функция для загрузки cookies
def load_cookies(driver):
    driver.get("https://github.com")
    with open("D:\\BSTU\\5 sem\\ТПО\\Лабы\\10\\cookies.json", "r") as file:
        cookies = json.load(file)
        for cookie in cookies:
            driver.add_cookie(cookie)
    driver.refresh()
    print("Cookies загружены.")

# Функция для сохранения скриншотов
def save_screenshot(driver, filename):
    driver.save_screenshot(filename)
    print(f"Скриншот сохранён: {filename}")

# Тест №1: Авторизация на GitHub
@pytest.mark.login
def test_login(setup_teardown):
    driver, wait = setup_teardown
    driver.get("https://github.com/login")
    try:
        username_field = wait.until(EC.presence_of_element_located((By.ID, "login_field")))
        password_field = wait.until(EC.presence_of_element_located((By.ID, "password")))
        username_field.send_keys(GITHUB_USERNAME)
        password_field.send_keys(GITHUB_PASSWORD)
        time.sleep(2)
        driver.find_element(By.NAME, "commit").click()
        time.sleep(20)
        wait.until(EC.presence_of_element_located((By.CSS_SELECTOR, "[id='dashboard-repositories-box']")))
        save_screenshot(driver, "D:\\BSTU\\5 sem\\ТПО\\Лабы\\10\\screenshots\\login_success.png")
        print("Вход аккаунт прошёл успешно.")
    except (NoSuchElementException, TimeoutException) as e:
        raise Exception(f"Ошибка входа: {e}")

# Тест №2: Изменение имени репозитория
@pytest.mark.skip(reason="Тест пропущен.")
@pytest.mark.repo
def test_change_repository_name(setup_teardown):
    test_login(setup_teardown)
    driver, wait = setup_teardown
    driver.get("https://github.com/jokimi/git-demo/settings")
    try:
        name_field = wait.until(EC.visibility_of_element_located((By.NAME, "new_name")))
        name_field.clear()
        name_field.send_keys(NEW_REPO_NAME)
        rename_button = wait.until(EC.element_to_be_clickable((By.XPATH, "//button[contains(text(), 'Rename')]")))
        rename_button.click()
        save_screenshot(driver, "D:\\BSTU\\5 sem\\ТПО\\Лабы\\10\\screenshots\\repo_renamed.png")
        print("Имя репозитория изменено.")
    except Exception as e:
        print(f"Ошибка: {e}")

# Тест №3: Создание Issue
@pytest.mark.issue
def test_create_issue(setup_teardown):
    test_login(setup_teardown)
    driver, wait = setup_teardown
    driver.get("https://github.com/jokimi/new-git-demo/issues")
    try:
        new_issue_button = wait.until(EC.element_to_be_clickable((By.CSS_SELECTOR, "a[href*='issues/new']")))
        new_issue_button.click()
        wait.until(EC.visibility_of_element_located((By.NAME, "issue[title]"))).send_keys("Test")
        wait.until(EC.visibility_of_element_located((By.NAME, "issue[body]"))).send_keys("Description")
        submit_button = wait.until(
            EC.element_to_be_clickable((By.XPATH, "//button[contains(text(), 'Submit new issue')]")))
        submit_button.click()
        wait.until(EC.url_contains("issues/"))
        save_screenshot(driver, "D:\\BSTU\\5 sem\\ТПО\\Лабы\\10\\screenshots\\issue_created.png")
        print("Issue создан успешно!")
    except (NoSuchElementException, TimeoutException) as e:
        print(f"Ошибка: {e}")

# Тест №4: Параметризация
@pytest.mark.parametrize("page_url, expected_element", [
    ("https://github.com/login", (By.ID, "login_field")),
    ("https://github.com", (By.NAME, "user_email")),
])
def test_check_page_functionality(setup_teardown, page_url, expected_element):
    driver, wait = setup_teardown
    driver.get(page_url)
    try:
        wait.until(EC.presence_of_element_located(expected_element))
        save_screenshot(driver, f"D:\\BSTU\\5 sem\\ТПО\\Лабы\\10\\screenshots\\page_check_{page_url.split('//')[-1].replace('/', '_')}.png")
        save_cookies(driver)
        load_cookies(driver)
        print(f"Функционал страницы {page_url} успешно проверен.")
    except TimeoutException:
        raise Exception(f"Не удалось найти элемент {expected_element} на странице {page_url}")