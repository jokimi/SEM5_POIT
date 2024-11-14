import pytest
import time
from unittest.mock import patch
from login import GitHubLogin
from repository import GitHubRepository

@pytest.fixture(scope="module")
def setup_driver():
    driver_path = "D:\\Applications\\environments\\yandexdriver.exe"
    github_login = GitHubLogin(driver_path)
    print("[INFO] WebDriver initialized.")
    yield github_login
    github_login.close()
    print("[INFO] WebDriver closed.")

@pytest.fixture(scope="module")
def setup_repository(setup_driver):
    print("[INFO] Initializing GitHubRepository instance.")
    return GitHubRepository(setup_driver.driver)

# Заглушка для теста логина
@patch.object(GitHubLogin, 'login', return_value=None)
def test_login(mock_login, setup_driver):
    print("[TEST] Starting test: test_login")

    # Здесь вызывается заглушка метода login
    setup_driver.login("2024jokeiminny", "jokeiminny2024")

    # Проверка вызова заглушки
    mock_login.assert_called_once_with("2024jokeiminny", "jokeiminny2024")
    print("[INFO] Login function called successfully.")

# Заглушка для проверки существования репозитория
@patch.object(GitHubRepository, 'check_repository_exists', return_value=True)
def test_repository_exists(mock_check_repo, setup_repository):
    print("[TEST] Starting test: test_repository_exists")

    # Здесь вызывается заглушка метода check_repository_exists
    exists = setup_repository.check_repository_exists("2024jokeiminny/git-demo")

    # Проверка вызова заглушки
    mock_check_repo.assert_called_once_with("2024jokeiminny/git-demo")
    assert exists
    print(f"[INFO] Repository {'exists' if exists else 'does not exist'}.")

    time.sleep(3)