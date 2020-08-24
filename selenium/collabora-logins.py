#!/usr/bin/python3
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.remote.remote_connection import LOGGER
import time
import logging

import config


LOGGER.setLevel(logging.INFO)

chrome_options = Options()
chrome_options.add_argument('--headless')

driver = webdriver.Chrome(chrome_options=chrome_options)
driver.delete_all_cookies()

#  = Login to gitlab through Phabricator =
driver.get("https://gitlab.collabora.com/")

# Sign in is a link, so easy to find by visible text.
driver.find_element_by_link_text('Sign in').click()

# The 'Phabricator' button, however, is not a link, but a button. Better use ID anyway.
driver.find_element_by_id('oauth-login-phabricator').click()

username = driver.find_element_by_name('ldap_username')
username.send_keys(config.username)

password = driver.find_element_by_name('ldap_password')
password.send_keys(config.password)

# All form fields have this nice shortcut, so we don't need to look for the
# Login or register button, we can just hit "enter" on the password.
password.submit()

# One more link, easy to use the visible text.
driver.find_element_by_link_text('Continue to Application').click()

assert driver.current_url == 'https://gitlab.collabora.com/public'

#time.sleep(2)

#  = Login to CRM through http basic auth =
driver.get("https://{}:{}@crm.collabora.com/".format(config.username, config.password))

username = driver.find_element_by_name('user_name')
username.send_keys(config.username)

password = driver.find_element_by_name('username_password')
password.send_keys(config.password)

# All form fields have this nice shortcut, so we don't need to look for the
# Login or register button, we can just hit "enter" on the password.
password.submit()

# Creds will also show up in the current URL.
assert driver.current_url == 'https://{}:{}@crm.collabora.com/index.php?module=Home&action=index'.format(config.username, config.password)
assert 'SUITECRM DASHBOARD' in driver.page_source

#time.sleep(2)

driver.quit()