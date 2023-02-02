import React from "react";
import Header from './components/header';
import './styles/audit.css';

export default function Audit() {
    const [audit, setAudit] = React.useState({
        "entry": [],
        "exit": []
    });

    const getAuditData = () => {
        fetch('http://localhost:8192/').then((res) => res.json().then((data) => {
            setAudit({
                "entry": data.EntryTimes,
                "exit": data.ExitTimes
            });
        }));
    }

    React.useEffect(() => {
        // Init one time
        getAuditData();

        // On-repeat
        setInterval(() => {
            getAuditData();
        }, 5 * 1000);
    }, [])

    let html = [];
    for (let i = 0; i < audit["entry"].length; i++) {
        html.push(
            <tr className="entry">
                <td>{audit["entry"][i]}</td>
                <td>Entry</td>
            </tr>
        );
    }

    for (let i = 0; i < audit["exit"].length; i++) {
        html.push(
            <tr className="exit">
                <td>{audit["exit"][i]}</td>
                <td>Exit</td>
            </tr>
        );
    }

    return (
        <>
            <Header/>
            <div className="audit-container">
                <table className="audit-table">
                    <thead>
                        <th>Timestamp</th>
                        <th>Type (Entry / Exit)</th>
                    </thead>
                    <tbody>
                        {html}
                    </tbody>
                </table>
            </div>
        </>
    )
}