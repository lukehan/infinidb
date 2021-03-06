/* Copyright (C) 2013 Calpont Corp.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation;
   version 2.1 of the License.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA. */

//  $Id: expressionstep.h 9632 2013-06-18 22:18:20Z xlou $


/** @file
 * class ExpStep interface
 */

#ifndef JOBLIST_EXPRESSION_STEP_H
#define JOBLIST_EXPRESSION_STEP_H

//#define NDEBUG
#include "jobstep.h"
#include "filter.h"


namespace execplan
{
// forward reference
class ReturnedColumn;
class SimpleColumn;
class WindowFunctionColumn;
};


namespace joblist
{

struct JobInfo;

class ExpressionStep : public JobStep
{
  public:
	// constructors
	ExpressionStep(const JobInfo&);
	// destructor constructors
	virtual ~ExpressionStep();

	// inherited methods
	void run();
	void join();
	const std::string toString() const;

	execplan::CalpontSystemCatalog::OID oid() const
	{ return fOids.empty() ? 0 : fOids.front(); }
	execplan::CalpontSystemCatalog::OID tableOid() const
	{ return fTableOids.empty() ? 0 : fTableOids.front(); }
	std::string alias() const { return fAliases.empty() ? "" : fAliases.front(); }
	std::string view() const { return fViews.empty() ? "" : fViews.front(); }
	std::string schema() const { return fSchemas.empty() ? "" : fSchemas.front(); }
	uint tableKey() const { return fTableKeys.empty() ? -1 : fTableKeys.front(); }
	uint columnKey() const { return fColumnKeys.empty() ? -1 : fColumnKeys.front(); }

	void expression(const execplan::SRCP exp, JobInfo& jobInfo);
	execplan::SRCP expression() const { return fExpression; }

	virtual void expressionFilter(const execplan::Filter* filter, JobInfo& jobInfo);
	virtual void expressionFilter(const execplan::ParseTree* filter, JobInfo& jobInfo);
	execplan::ParseTree* expressionFilter() const { return fExpressionFilter; }

	void expressionId(uint64_t eid) { fExpressionId = eid; }
	uint64_t expressionId() const { return fExpressionId; }

	const std::vector<execplan::CalpontSystemCatalog::OID>& tableOids() const { return fTableOids; }
	const std::vector<execplan::CalpontSystemCatalog::OID>& oids() const { return fOids; }
	const std::vector<std::string>& aliases() const { return fAliases; }
	const std::vector<std::string>& views() const { return fViews; }
	const std::vector<std::string>& schemas() const { return fSchemas; }
	const std::vector<uint>& tableKeys() const { return fTableKeys; }
	const std::vector<uint>& columnKeys() const { return fColumnKeys; }

	std::vector<execplan::CalpontSystemCatalog::OID>& tableOids() { return fTableOids; }
	std::vector<execplan::CalpontSystemCatalog::OID>& oids() { return fOids; }
	std::vector<std::string>& aliases() { return fAliases; }
	std::vector<std::string>& views() { return fViews; }
	std::vector<std::string>& schemas() { return fSchemas; }
	std::vector<uint>& tableKeys() { return fTableKeys; }
	std::vector<uint>& columnKeys() { return fColumnKeys; }

	virtual void updateInputIndex(std::map<uint, uint>& indexMap, const JobInfo& jobInfo);
	virtual void updateOutputIndex(std::map<uint, uint>& indexMap, const JobInfo& jobInfo);
	virtual void updateColumnOidAlias(JobInfo& jobInfo);

	std::vector<execplan::ReturnedColumn*>& columns() { return fColumns; }
	void substitute(uint64_t, const SSC&);

	void selectFilter(bool b) { fSelectFilter = b; }
	bool selectFilter() const { return fSelectFilter; }

	void associatedJoinId(uint64_t i) { fAssociatedJoinId = i; }
	uint64_t associatedJoinId() const { return fAssociatedJoinId; }

  protected:
	virtual void addColumn(execplan::ReturnedColumn* rc, JobInfo& jobInfo);
	virtual void populateColumnInfo(execplan::ReturnedColumn* rc, JobInfo& jobInfo);
	virtual void populateColumnInfo(execplan::SimpleColumn* sc, JobInfo& jobInfo);
	virtual void populateColumnInfo(execplan::WindowFunctionColumn* wc, JobInfo& jobInfo);


	// expression
	execplan::SRCP                                   fExpression;
	execplan::ParseTree*                             fExpressionFilter;
	uint64_t                                         fExpressionId;

	// columns accessed
	std::vector<execplan::CalpontSystemCatalog::OID> fTableOids;
	std::vector<execplan::CalpontSystemCatalog::OID> fOids;
	std::vector<std::string>                         fAliases;
	std::vector<std::string>                         fViews;
	std::vector<std::string>                         fSchemas;
	std::vector<uint>                                fTableKeys;
	std::vector<uint>                                fColumnKeys;
	std::vector<execplan::ReturnedColumn*>           fColumns;

  private:
	// disable copy constructor
	// Cannot copy fColumns, which depends on fExpressionFilter.
	ExpressionStep(const ExpressionStep&);

	// for VARBINARY, only support limited number of functions: like hex(), length(), etc.
	bool                                             fVarBinOK;

	// @bug 3780, for select filter
	bool                                             fSelectFilter;

	// @bug 3037, outer join with additional comparison
	uint64_t                                         fAssociatedJoinId;

	// @bug 4531, for window function in IN/EXISTS sub-query.
	std::vector<execplan::SSC>                                   fSubstitutes;
	std::map<execplan::SimpleColumn*, execplan::ReturnedColumn*> fSubMap;
};

}

#endif //  JOBLIST_EXPRESSION_STEP_H

