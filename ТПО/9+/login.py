from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.chrome.service import Service
import time

class GitHubLogin:
    def __init__(self, driver_path):
        service = Service(driver_path)
        self.driver = webdriver.Chrome(service=service)

    def login(self, username, password):
        self.driver.get("https://github.com/login")
        time.sleep(2)

        username_input = self.driver.find_element(By.ID, "login_field")
        username_input.send_keys(username)

        password_input = self.driver.find_element(By.ID, "password")
        password_input.send_keys(password)
        password_input.send_keys(Keys.RETURN)

        time.sleep(2)

    def close(self):
        self.driver.quit()