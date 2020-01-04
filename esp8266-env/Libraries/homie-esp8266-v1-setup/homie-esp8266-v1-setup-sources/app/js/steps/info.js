'use strict';

import 'whatwg-fetch';

import React from 'react';

export default class InfoStep extends React.Component {
  constructor (props) {
    super(props);
    this.state = {
      loading: true,
      info: {}
    };
  }

  componentDidMount () {
    let interval;
    let done = false;
    let deviceinfo = () => {
      window.fetch(`${this.props.baseApi}/device-info`).then((res) => {
        if (res.ok && !done) {
          done = true;
          window.clearInterval(interval);
          return res.json();
        }
      }).then((json) => {
        this.setState({
          loading: false,
          info: json
        });
      });
    };

    interval = window.setInterval(deviceinfo, 5 * 1000);
    deviceinfo();
  }

  handleNextButton (e) {
    e.preventDefault();

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
                Gathering device information...
              </div>
            );
          } else {
            return (
              <div className='content'>
                <p>
                  Here are some information about the device you are about to configure:
                </p>

                <ul>
                  <li><span className='icon is-small'><i className='fa fa-hashtag'></i></span> Device ID: { this.state.info.device_id }</li>
                  <li><span className='icon is-small'><i className='fa fa-tag'></i></span> Homie version: { this.state.info.homie_version }</li>
                  <li><span className='icon is-small'><i className='fa fa-font'></i></span> Firmware name: { this.state.info.firmware.name }</li>
                  <li><span className='icon is-small'><i className='fa fa-tag'></i></span> Firmware version: { this.state.info.firmware.version }</li>
                  <li>
                    <span className='icon is-small'><i className='fa fa-cubes'></i></span> Nodes:
                    <table className='table'>
                      <thead>
                        <tr>
                          <th>ID</th>
                          <th>Type</th>
                        </tr>
                      </thead>
                      <tbody>
                        { this.state.info.nodes.map((node, i) => {
                          return (
                            <tr key={i}>
                              <td>{ node.id }</td>
                              <td>{ node.type }</td>
                            </tr>
                          );
                        }) }
                      </tbody>
                    </table>
                  </li>
                </ul>

                <a className='button is-primary' onClick={(e) => { this.handleNextButton(e); } }>Next</a>
              </div>
            );
          }
        })()}
      </div>
    );
  }
}

if (process.env.NODE_ENV !== 'production') { // for Preact
  InfoStep.propTypes = {
    baseApi: React.PropTypes.string.isRequired,
    nextStep: React.PropTypes.func.isRequired
  };
}
