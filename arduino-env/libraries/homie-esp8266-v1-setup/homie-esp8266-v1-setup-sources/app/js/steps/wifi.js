'use strict';

import 'whatwg-fetch';

import React from 'react';

export default class WifiStep extends React.Component {
  constructor (props) {
    super(props);
    this.state = {
      loading: false,
      networks: { networks: [] },
      buttonDisabled: true,
      selectedSsid: null,
      showSsidInput: false,
      showPasswordInput: false,
      showPassword: false
    };
  }

  componentDidMount () {
    let interval;
    let done = false;
    let networks = () => {
      window.fetch(`${this.props.baseApi}/networks`).then((res) => {
        if (res.ok && !done) {
          done = true;
          window.clearInterval(interval);
          return res.json();
        }
      }).then((json) => {
        this.setState({
          loading: false,
          networks: json
        });
      });
    };

    interval = window.setInterval(networks, 5 * 1000);
    networks();
  }

  handleSelectChange (e) {
    if (e.target.value === 'select') {
      this.setState({ showSsidInput: false, showPasswordInput: false, selectedSsid: null, buttonDisabled: true });
    } else if (e.target.value === 'other') {
      this.setState({ showSsidInput: true, showPasswordInput: true, selectedSsid: null, buttonDisabled: false });
    } else {
      let data = e.target.options[e.target.selectedIndex].dataset;
      this.setState({ showSsidInput: false, showPasswordInput: data.open === 'no', selectedSsid: data.ssid, buttonDisabled: false });
    }
  }

  handleHiddenChange (e) {
    this.setState({ showPassword: e.target.checked });
  }

  handleFormSubmit (e) {
    e.preventDefault();

    let creds = {};

    if (this.state.selectedSsid) {
      creds.ssid = this.state.selectedSsid;
    } else {
      creds.ssid = this.refs.ssid.value;
    }
    creds.password = this.refs.password.value;

    this.props.setWifiCreds(creds);
    this.props.nextStep();
  }

  render () {
    return (
      <div>
        {(() => {
          if (this.state.loading) {
            return (
              <div className='notification is-info'>
                <span className='button is-info is-loading'>Loading</span>
                Gathering available networks...
              </div>
            );
          } else {
            this.state.networks.networks.sort(function (networkA, networkB) {
              if (networkA.rssi > networkB.rssi) {
                return -1;
              } else if (networkA.rssi < networkB.rssi) {
                return 1;
              } else {
                return 0;
              }
            });

            let networks = this.state.networks.networks.map(function (network) {
              if (network.rssi <= -100) {
                network.signalQuality = 0;
              } else if (network.rssi >= -50) {
                network.signalQuality = 100;
              } else {
                network.signalQuality = 2 * (network.rssi + 100);
              }

              switch (network.encryption) {
                case 'wep':
                  network.encryption = 'WEP';
                  break;
                case 'wpa':
                  network.encryption = 'WPA';
                  break;
                case 'wpa2':
                  network.encryption = 'WPA2';
                  break;
                case 'none':
                  network.encryption = 'Open';
                  break;
                case 'auto':
                  network.encryption = 'Automatic';
                  break;
              }
              return network;
            });

            return (
              <div className='content'>
                <p>
                  Select the Wi-Fi network to connect to:
                </p>

                <form onSubmit={ (e) => this.handleFormSubmit(e) }>
                  <label className='label' htmlFor='wifi_network'>Network</label>
                  <p className='control'>
                    <span className='select'>
                      <select ref='select' onChange={ (e) => { this.handleSelectChange(e); } } id='wifi_network'>
                        <option value='select' key='select'>Select...</option>
                        { networks.map((network, i) => {
                          return (
                            <option value={i} data-ssid={ network.ssid } data-open={ network.encryption === 'Open' ? 'yes' : 'no' } onSelect={ (e) => { window.alert(network.ssid); } } key={i}>
                              { network.ssid } - { network.encryption } ({ network.signalQuality }%)
                            </option>
                          );
                        }) }

                        <option value='other' key='other'>Other/Hidden network</option>
                      </select>
                    </span>
                  </p>

                  {(() => {
                    if (this.state.showSsidInput) {
                      return (
                        <div>
                          <label className='label' htmlFor='wifi_ssid'>Wi-Fi SSID</label>
                          <p className='control'>
                            <input ref='ssid' className='input' type='text' placeholder='SSID' maxLength='32' id='wifi_ssid' required />
                            <span className='help'>Required.</span>
                          </p>
                        </div>
                      );
                    }
                  })()}

                  {(() => {
                    if (this.state.showPasswordInput) {
                      return (
                        <div>
                          <label className='label' htmlFor='wifi_password'>Wi-Fi password</label>
                          <p className='control is-grouped'>
                            <input ref='password' className='input' type={ this.state.showPassword ? 'text' : 'password' } placeholder='Password' id='wifi_password' />
                            <label className='checkbox'>
                              <input type='checkbox' onChange={ (e) => this.handleHiddenChange(e) } />
                              Show password
                            </label>
                          </p>
                          <span className='help'>Optional. Leave blank if open network.</span>
                          <br/>
                        </div>
                      );
                    }
                  })()}

                  <p className='control'>
                    <button type='submit' disabled={ this.state.buttonDisabled } className='button is-primary'>Next</button>
                  </p>
                </form>
              </div>
            );
          }
        })()}
      </div>
    );
  }
}

if (process.env.NODE_ENV !== 'production') { // for Preact
  WifiStep.propTypes = {
    baseApi: React.PropTypes.string.isRequired,
    nextStep: React.PropTypes.func.isRequired,
    setWifiCreds: React.PropTypes.func.isRequired
  };
}
