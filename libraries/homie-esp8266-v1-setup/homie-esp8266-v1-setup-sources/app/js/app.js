'use strict';

import 'whatwg-fetch';

import React from 'react';
import ReactDOM from 'react-dom';

import ConnectionStep from './steps/connection';
import InfoStep from './steps/info';
import WifiStep from './steps/wifi';
import MqttStep from './steps/mqtt';
import DetailsStep from './steps/details';
import SendingStep from './steps/sending';

const BASE_API = 'http://homie.config';

const STEP_CONNECTION = 1;
const STEP_INFO = 2;
const STEP_WIFI = 3;
const STEP_MQTT = 4;
const STEP_DETAILS = 5;
const STEP_SENDING = 6;

class App extends React.Component {
  constructor (props) {
    super(props);
    this.state = {
      step: STEP_CONNECTION,
      name: null,
      deviceId: null,
      wifi: { },
      mqtt: { },
      ota: { }
    };
  }

  nextStep () {
    this.setState({ step: this.state.step + 1 });
  }

  setName (name) {
    this.setState({ name });
  }

  setDeviceId (deviceId) {
    this.setState({ deviceId });
  }

  setWifiCreds (creds) {
    this.setState({ wifi: creds });
  }

  setMqttCreds (creds) {
    this.setState({ mqtt: creds });
  }

  setOtaCreds (creds) {
    this.setState({ ota: creds });
  }

  sendConfig () {
    let body = {
      name: this.state.name,
      wifi: this.state.wifi,
      mqtt: this.state.mqtt,
      ota: this.state.ota
    };

    if (this.state.deviceId !== null) body['device_id'] = this.state.deviceId;

    let options = {
      method: 'PUT',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(body)
    };

    return new Promise((resolve, reject) => {
      window.fetch(`${BASE_API}/config`, options).then((res) => {
        return res.json();
      }).then((json) => {
        if (json.success) {
          resolve();
        } else {
          reject();
        }
      }).catch(() => {
        reject();
      });
    });
  }

  render () {
    let Step;

    switch (this.state.step) {
      case STEP_CONNECTION:
        Step = ConnectionStep;
        break;
      case STEP_INFO:
        Step = InfoStep;
        break;
      case STEP_WIFI:
        Step = WifiStep;
        break;
      case STEP_MQTT:
        Step = MqttStep;
        break;
      case STEP_DETAILS:
        Step = DetailsStep;
        break;
      case STEP_SENDING:
        Step = SendingStep;
        break;
    }

    return (
      <div className='container'>
        <h1 className='title'>Homie for ESP8266</h1>
        <h2 className='subtitle'>Set up your device.</h2>

        <div className='tabs is-centered'>
          <ul>
            <li className={this.state.step === STEP_CONNECTION ? 'is-active' : ''}><a href='' onClick={(e) => { e.preventDefault(); } }><i className='fa fa-heartbeat'></i></a></li>
            <li className={this.state.step === STEP_INFO ? 'is-active' : ''}><a href='' onClick={(e) => { e.preventDefault(); } }><i className='fa fa-info'></i></a></li>
            <li className={this.state.step === STEP_WIFI ? 'is-active' : ''}><a href='' onClick={(e) => { e.preventDefault(); } }><i className='fa fa-wifi'></i></a></li>
            <li className={this.state.step === STEP_MQTT ? 'is-active' : ''}><a href='' onClick={(e) => { e.preventDefault(); } }><i className='fa fa-signal'></i></a></li>
            <li className={this.state.step === STEP_DETAILS ? 'is-active' : ''}><a href='' onClick={(e) => { e.preventDefault(); } }><i className='fa fa-cogs'></i></a></li>
            <li className={this.state.step === STEP_SENDING ? 'is-active' : ''}><a href='' onClick={(e) => { e.preventDefault(); } }><i className='fa fa-rocket'></i></a></li>
          </ul>
        </div>

        <Step baseApi={BASE_API} mqttConfig={this.state.mqtt} nextStep={() => this.nextStep()} setWifiCreds={(creds) => this.setWifiCreds(creds)} setMqttCreds={(creds) => this.setMqttCreds(creds)} setName={(name) => this.setName(name)} setDeviceId={(deviceId) => this.setDeviceId(deviceId)} setOtaCreds={(creds) => this.setOtaCreds(creds)} sendConfig={() => this.sendConfig()} />
      </div>
    );
  }
}

ReactDOM.render(<App/>, document.getElementById('app'));
