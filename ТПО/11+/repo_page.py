from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

class RepositorySettingsPage:
    def __init__(self, driver):
        self.driver = driver
        self.wait = WebDriverWait(driver, 10)
        self.name_field = (By.NAME, "new_name")
        self.rename_button = (By.XPATH, "//button[contains(text(), 'Rename')]")

    def open(self, repo_url):
        self.driver.get(f"{repo_url}/settings")

    def change_name(self, new_name):
        name_input = self.wait.until(EC.visibility_of_element_located(self.name_field))
        name_input.clear()
        name_input.send_keys(new_name)
        self.wait.until(EC.element_to_be_clickable(self.rename_button)).click()