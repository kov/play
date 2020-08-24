#!/usr/bin/python3
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

driver = webdriver.Chrome()

while True:
    driver.delete_all_cookies()

    driver.get("https://www.enqueteaovivo.com.br/")
    
    votar = driver.find_element_by_link_text('VOTAR')
    votar.send_keys(Keys.RETURN)
    
    assert 'ESCOLHA SEU CANDIDATO' in driver.page_source

    marina = driver.find_element_by_xpath('/html/body/div[4]/div/div[6]/div[1]/img')

    assert marina.get_attribute('src').endswith('/marina2.png')

    marina.click()

    antes = Select(driver.find_element_by_id('antes'))
    antes.select_by_visible_text('Marina Silva')

    confirmar = driver.find_element_by_id('confirma')
    confirmar.click()

    retorno = WebDriverWait(driver, 10).until(
        EC.text_to_be_present_in_element((By.ID, "retorno"),
        "Voto efetuado com sucesso em Marina Silva!")
    )
