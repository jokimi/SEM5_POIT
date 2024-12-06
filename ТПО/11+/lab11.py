import unittest
from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
from login_page import LoginPage
from repo_page import RepositorySettingsPage
from issue_page import IssuePage

class GitHubTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        options = Options()
        cls.driver_path = "D:\\Applications\\environments\\yandexdriver.exe"
        cls.service = Service(cls.driver_path)
        cls.driver = webdriver.Chrome(service=cls.service, options=options)
        cls.driver.maximize_window()

    @classmethod
    def tearDownClass(cls):
        cls.driver.quit()

    def test_1_login(self):
        login_page = LoginPage(self.driver)
        login_page.open()
        login_page.login("mylogin", "password")

    def test_2_change_repository_name(self):
        repo_settings = RepositorySettingsPage(self.driver)
        repo_settings.open("https://github.com/mylogin/git-demo")
        repo_settings.change_name("new-git-demo")

    def test_3_create_issue(self):
        issue_page = IssuePage(self.driver)
        issue_page.open("https://github.com/mylogin/new-git-demo")
        issue_page.create_issue("Test Issue", "This is a test issue.")

if __name__ == "__main__":
    unittest.main()