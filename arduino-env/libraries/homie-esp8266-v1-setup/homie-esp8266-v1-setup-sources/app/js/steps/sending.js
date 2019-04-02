'use strict';

import React from 'react';

export default class SendingStep extends React.Component {
  constructor (props) {
    super(props);
    this.state = {
      loading: true,
      success: null
    };
  }

  componentDidMount () {
    this.props.sendConfig().then(() => {
      this.setState({ loading: false, success: true });
    }).catch(() => {
      this.setState({ loading: false, success: false });
    });
  }

  render () {
    return (
      <div>
        {(() => {
          if (this.state.loading) {
            return (
              <div className='notification is-info'>
                <span className='button is-info is-loading'>Loading</span>
                Sending configuration...
              </div>
            );
          } else {
            if (this.state.success) {
              return (
                <div className='notification is-success'>
                  The configuration was sent. Your device will reboot.
                </div>
              );
            } else {
              return (
                <div className='notification is-danger'>
                  There was an error while sending the configuration. Please retry.
                </div>
              );
            }
          }
        })()}
      </div>
    );
  }
}

if (process.env.NODE_ENV !== 'production') { // for Preact
  SendingStep.propTypes = {
    sendConfig: React.PropTypes.func.isRequired
  };
}
