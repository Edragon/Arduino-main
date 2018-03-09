'use strict';

import 'whatwg-fetch';

import React from 'react';

export default class ConnectionStep extends React.Component {
  constructor (props) {
    super(props);
  }

  componentDidMount () {
    let interval;
    let done = false;
    let heartbeat = () => {
      window.fetch(`${this.props.baseApi}/heart`).then((res) => {
        if (res.ok && !done) {
          done = true; // prevent multiple request to trigger multiple nextStep
          window.clearInterval(interval);
          this.props.nextStep();
        }
      });
    };

    interval = window.setInterval(heartbeat, 5 * 1000);
    heartbeat();
  }

  render () {
    return (
      <div>
        <div className='content'>
          <p>
            Connect to your device Wi-Fi AP.
            If the AP is named <strong>Homie-1234abcd</strong>,
            the password is <strong>1234abcd</strong>.
          </p>
        </div>
        <div className='notification is-info'>
          <span className='button is-info is-loading'>Loading</span>
          Waiting for the device...
        </div>
      </div>
    );
  }
}

if (process.env.NODE_ENV !== 'production') { // for Preact
  ConnectionStep.propTypes = {
    baseApi: React.PropTypes.string.isRequired,
    nextStep: React.PropTypes.func.isRequired
  };
}
