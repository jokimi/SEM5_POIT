from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time

class LoginPage:
    def __init__(self, driver):
        self.driver = driver
        self.wait = WebDriverWait(driver, 10)
        self.username_field = (By.ID, "login_field")
        self.password_field = (By.ID, "password")
        self.submit_button = (By.NAME, "commit")
        self.dashboard = (By.CSS_SELECTOR, "[id='dashboard-repositories-box']")

    def open(self):
        self.driver.get("https://github.com/login")

    def login(self, username, password):
        self.wait.until(EC.presence_of_element_located(self.username_field)).send_keys(username)
        self.wait.until(EC.presence_of_element_located(self.password_field)).send_keys(password)
        self.driver.find_element(*self.submit_button).click()
        time.sleep(5)
        self.wait.until(EC.presence_of_element_located(self.dashboard))