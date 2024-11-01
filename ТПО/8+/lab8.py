from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.common.exceptions import NoSuchElementException, TimeoutException
import time
import os

GITHUB_USERNAME = "login"
GITHUB_PASSWORD = "password"
NEW_REPO_NAME = 'new-git-demo'

if not GITHUB_USERNAME or not GITHUB_PASSWORD:
    raise ValueError("GITHUB_USERNAME and GITHUB_PASSWORD environment variables must be set.")

yandex_options = Options()
# yandex_options.add_argument("--headless")

driver_path = "D:\\Applications\\environments\\yandexdriver.exe"
service = Service(driver_path)
driver = webdriver.Chrome(service=service, options=yandex_options)
wait = WebDriverWait(driver, 10)

# Тест №1: Авторизация на GitHub

def login_to_github():
    driver.get("https://github.com/login")
    try:
        username_field = wait.until(EC.presence_of_element_located((By.ID, "login_field")))
        password_field = wait.until(EC.presence_of_element_located((By.ID, "password")))
        username_field.send_keys(GITHUB_USERNAME)
        password_field.send_keys(GITHUB_PASSWORD)
        time.sleep(2)
        driver.find_element(By.NAME, "commit").click()
        driver.find_element(By.CSS_SELECTOR, "button[type='submit']").click()
        time.sleep(20)
        wait.until(EC.presence_of_element_located((By.CSS_SELECTOR, "[id='dashboard-repositories-box']")))
        print("Вход аккаунт прошел успешно.")
    except (NoSuchElementException, TimeoutException) as e:
        raise Exception(f"Ошибка входа: {e}")

# Тест №2: Изменение имени репозитория

def change_repository_name():
    driver.get("https://github.com/login/git-demo/settings")
    try:
        name_field = wait.until(EC.visibility_of_element_located((By.NAME, "new_name")))
        name_field.clear()
        name_field.send_keys(NEW_REPO_NAME)
        rename_button = wait.until(EC.element_to_be_clickable((By.XPATH, "//button[contains(text(), 'Rename')]")))
        rename_button.click()
        print("Имя репозитория изменено.")
        time.sleep(5)
        print("Тест прошел успешно: имя репозитория изменено.")
    except Exception as e:
        print(f"Ошибка: {e}")
    finally:
        driver.quit()

# Тест №3: Создание Issue

def create_issue():
    driver.get("https://github.com/login/new-git-demo/issues")
    try:
        new_issue_button = wait.until(EC.element_to_be_clickable((By.CSS_SELECTOR, "a[href*='issues/new']")))
        new_issue_button.click()
    except (NoSuchElementException, TimeoutException):
        print("Не удалось найти кнопку для создания нового Issue")
        return
    wait.until(EC.visibility_of_element_located((By.NAME, "issue[title]"))).send_keys("Test")
    wait.until(EC.visibility_of_element_located((By.NAME, "issue[body]"))).send_keys("Description")
    try:
        submit_button = wait.until(EC.element_to_be_clickable((By.XPATH, "//button[contains(text(), 'Submit new issue')]")))
        submit_button.click()
    except (NoSuchElementException, TimeoutException):
        print("Не удалось найти кнопку Submit")
    wait.until(EC.url_contains("issues/"))
    print("Issue создан успешно!")

# Тест №4: Выбор радиокнопки

def select_radio_button():
    driver.get("https://demoqa.com/radio-button")
    try:
        yes_radio_button = wait.until(EC.element_to_be_clickable((By.XPATH, "//label[@for='yesRadio']")))
        driver.execute_script("arguments[0].click();", yes_radio_button)
        assert yes_radio_button.find_element(By.XPATH, "//input[@id='yesRadio']").is_selected(), "The 'Yes' radio button should be selected."
        print("Тест прошел успешно: радиокнопка 'Yes' выбрана.")
    except Exception as e:
        print(f"Ошибка: {e}")
    finally:
        time.sleep(5)
        driver.quit()

# Выполнение тестов

try:
    login_to_github()
    change_repository_name()
    #create_issue()
    #select_radio_button()
except Exception as e:
    print(f"Ошибка: {e}")
finally:
    driver.quit()