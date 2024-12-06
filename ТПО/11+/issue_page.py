from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

class IssuePage:
    def __init__(self, driver):
        self.driver = driver
        self.wait = WebDriverWait(driver, 10)
        self.new_issue_button = (By.CSS_SELECTOR, "a[href*='issues/new']")
        self.issue_title = (By.NAME, "issue[title]")
        self.issue_body = (By.NAME, "issue[body]")
        self.submit_button = (By.XPATH, "//button[contains(text(), 'Submit new issue')]")

    def open(self, repo_url):
        self.driver.get(f"{repo_url}/issues")

    def create_issue(self, title, body):
        self.wait.until(EC.element_to_be_clickable(self.new_issue_button)).click()
        self.wait.until(EC.presence_of_element_located(self.issue_title)).send_keys(title)
        self.wait.until(EC.presence_of_element_located(self.issue_body)).send_keys(body)
        self.wait.until(EC.element_to_be_clickable(self.submit_button)).click()