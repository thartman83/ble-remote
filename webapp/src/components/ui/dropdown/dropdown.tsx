import { ChangeEvent } from "react";
import "./dropdown.css";

export type DropdownChangeEvent = ChangeEvent<HTMLSelectElement>;

export type DropdownProps = {
  value: string,
  label: string,
  changeHandler: (e: DropdownChangeEvent) => void,
};

const Dropdown = ({value, label, changeHandler}: DropdownProps) => {
  return (
    <div className="dropdown-group">
      <label>{label}</label>
      <select onChange={changeHandler} defaultValue={value}>
        <option disabled={true} className="placeholder">
        - Select Wifi Network -
        </option>
        <option>Another Option</option>
      </select>
      </div>
  );
};

export default Dropdown;
