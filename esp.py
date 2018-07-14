from flask import Flask, render_template, request, flash
from flask_wtf import Form
from wtforms import DecimalField, SubmitField, validators, ValidationError, SubmitField
import pickle
import requests
import json

app = Flask(__name__)
app.secret_key = 'development key'

loaded_model = pickle.load(open('g.sav', 'rb'))

class InputForm(Form):
    moisture = DecimalField("Moisture: ", [validators.Required()])
    temperature = DecimalField("Temperature (%): ",[validators.Required()])
    humidity = DecimalField("Humidity (%): ",[validators.Required()])


@app.route('/yoESP', methods = ['POST'])
def ESPHandler():
	content = request.get_json()
	moisture = content['moisture']
	temperature = content['temperature']
	humidity = content['humidity']	
	return 'NodeMCU'
		

@app.route('/', methods =['GET', 'POST'])
def home():
    form = InputForm(request.form)
    if request.method == 'POST':
        if form.validate() == False:
            flash('All fields are required.')
            return render_template('form.html', form = form)
        else:
            moisture = request.form['moisture']
            temperature = request.form['temperature']
            humidity = request.form['humidity']

            input_values = [[(moisture), (temperature), (humidity)]]
            result = loaded_model.predict(input_values)
            if result == 0:
                crop = 'Corn'
            elif result == 1:
                crop = 'Maize'
            elif result == 2:
                crop = 'Potato'
            elif result == 3:
                crop = 'Rice'
            elif result == 4:
                crop = 'Sugarcane'
            elif result == 5:
                crop = 'Tomato'
            else:
                crop = 'Wheat'
            return render_template('thatCrop.html', crop = crop)
    if request.method == 'GET':
        return render_template('form.html', form = form)

@app.route('/readycode', methods =['POST','GET'])
def ready():
	if request.method =='POST':
		#r = requests.post("http://192.168.100.98:80/body")
		url = 'http://192.168.100.98:80/body'
		payload={'fruit':'apple'}
		headers = {'content-type': 'application/json'}
		r = requests.post(url, data = json.dumps(payload), headers=headers)
		return render_template('intermediate.html')
	else:
		return 'No'
		
		
		
if __name__ == "__main__":	
	app.run(host = '0.0.0.0', port = 8090)
	
'''
	content = request.get_json()
	
	moisture = req_data['moisture']
	temperature = req_data['temperature']
	humidity = req_data['humidity']
	
'''