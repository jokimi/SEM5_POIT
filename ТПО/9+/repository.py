from selenium.webdriver.common.by import By

class GitHubRepository:
    def __init__(self, driver):
        self.driver = driver

    def check_repository_exists(self, repo_name):
        self.driver.get(f"https://github.com/{repo_name}")
        return "404" not in self.driver.title